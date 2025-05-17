// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiHeroComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "Input/MiraiMappableConfigPair.h"
#include "MiraiLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MiraiPlayerController.h"
#include "Player/MiraiPlayerState.h"
#include "Player/MiraiLocalPlayer.h"
#include "Character/MiraiPawnExtensionComponent.h"
#include "Character/MiraiPawnData.h"
#include "Character/MiraiCharacter.h"
#include "AbilitySystem/MiraiAbilitySystemComponent.h"
#include "Input/MiraiInputConfig.h"
#include "Input/MiraiInputComponent.h"
#include "Camera/MiraiCameraComponent.h"
#include "MiraiGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/MiraiCameraMode.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiHeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace MiraiHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UMiraiHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UMiraiHeroComponent::NAME_ActorFeatureName("Hero");

UMiraiHeroComponent::UMiraiHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UMiraiHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogMirai, Error, TEXT("[UMiraiHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("MiraiHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("MiraiHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UMiraiHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == MiraiGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == MiraiGameplayTags::InitState_Spawned && DesiredState == MiraiGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AMiraiPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AMiraiPlayerController* MiraiPC = GetController<AMiraiPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !MiraiPC || !MiraiPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == MiraiGameplayTags::InitState_DataAvailable && DesiredState == MiraiGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AMiraiPlayerState* MiraiPS = GetPlayerState<AMiraiPlayerState>();

		return MiraiPS && Manager->HasFeatureReachedInitState(Pawn, UMiraiPawnExtensionComponent::NAME_ActorFeatureName, MiraiGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == MiraiGameplayTags::InitState_DataInitialized && DesiredState == MiraiGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UMiraiHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == MiraiGameplayTags::InitState_DataAvailable && DesiredState == MiraiGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AMiraiPlayerState* MiraiPS = GetPlayerState<AMiraiPlayerState>();
		if (!ensure(Pawn && MiraiPS))
		{
			return;
		}

		const UMiraiPawnData* PawnData = nullptr;

		if (UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UMiraiPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(MiraiPS->GetMiraiAbilitySystemComponent(), MiraiPS);
		}

		if (AMiraiPlayerController* MiraiPC = GetController<AMiraiPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UMiraiCameraComponent* CameraComponent = UMiraiCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UMiraiHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UMiraiPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == MiraiGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UMiraiHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { MiraiGameplayTags::InitState_Spawned, MiraiGameplayTags::InitState_DataAvailable, MiraiGameplayTags::InitState_DataInitialized, MiraiGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UMiraiHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UMiraiPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(MiraiGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UMiraiHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UMiraiHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UMiraiLocalPlayer* LP = Cast<UMiraiLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UMiraiPawnData* PawnData = PawnExtComp->GetPawnData<UMiraiPawnData>())
		{
			if (const UMiraiInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					UInputMappingContext* IMC = Mapping.InputMapping.Get();
					if (!IMC)
					{
						UE_LOG( LogMirai, Warning, TEXT( "%s: Failed to load InputMappingContext from soft reference." ), ANSI_TO_TCHAR(__FUNCTION__) );
						continue;
					}
					
					if (Mapping.bRegisterWithSettings)
					{
						if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
						{
								Settings->RegisterInputMappingContext(IMC);
						}
							
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						// Actually add the config to the local player							
						Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
					}
				}

				// The Mirai Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UMiraiInputComponent or modify this component accordingly.
				UMiraiInputComponent* MiraiIC = Cast<UMiraiInputComponent>(PlayerInputComponent);
				if (ensureMsgf(MiraiIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMiraiInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					MiraiIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					MiraiIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					MiraiIC->BindNativeAction(InputConfig, MiraiGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ true);
					MiraiIC->BindNativeAction(InputConfig, MiraiGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ true);
					MiraiIC->BindNativeAction(InputConfig, MiraiGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ true);
					MiraiIC->BindNativeAction(InputConfig, MiraiGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ true);
					MiraiIC->BindNativeAction(InputConfig, MiraiGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ true);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UMiraiHeroComponent::AddAdditionalInputConfig(const UMiraiInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UMiraiInputComponent* MiraiIC = Pawn->FindComponentByClass<UMiraiInputComponent>();
		if (ensureMsgf(MiraiIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMiraiInputComponent or a subclass of it.")))
		{
			MiraiIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UMiraiHeroComponent::RemoveAdditionalInputConfig(const UMiraiInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UMiraiHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UMiraiHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UMiraiAbilitySystemComponent* MiraiASC = PawnExtComp->GetMiraiAbilitySystemComponent())
			{
				MiraiASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UMiraiHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UMiraiAbilitySystemComponent* MiraiASC = PawnExtComp->GetMiraiAbilitySystemComponent())
		{
			MiraiASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UMiraiHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (AMiraiPlayerController* MiraiController = Cast<AMiraiPlayerController>(Controller))
	{
		MiraiController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UMiraiHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UMiraiHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * MiraiHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * MiraiHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UMiraiHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AMiraiCharacter* Character = GetPawn<AMiraiCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UMiraiHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (AMiraiPlayerController* Controller = Cast<AMiraiPlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}	
	}
}

TSubclassOf<UMiraiCameraMode> UMiraiHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UMiraiPawnData* PawnData = PawnExtComp->GetPawnData<UMiraiPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void UMiraiHeroComponent::SetAbilityCameraMode(TSubclassOf<UMiraiCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UMiraiHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

