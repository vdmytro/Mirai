// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPlayerController.h"
#include "CommonInputTypeEnum.h"
#include "Components/PrimitiveComponent.h"
#include "MiraiLogChannels.h"
#include "MiraiCheatManager.h"
#include "MiraiPlayerState.h"
#include "Camera/MiraiPlayerCameraManager.h"
#include "UI/MiraiHUD.h"
#include "AbilitySystem/MiraiAbilitySystemComponent.h"
#include "EngineUtils.h"
#include "MiraiGameplayTags.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameInstance.h"
#include "AbilitySystemGlobals.h"
#include "CommonInputSubsystem.h"
#include "MiraiLocalPlayer.h"
//#include "GameModes/MiraiGameState.h"
#include "Settings/MiraiSettingsLocal.h"
#include "Settings/MiraiSettingsShared.h"
//#include "Replays/MiraiReplaySubsystem.h"
//#include "ReplaySubsystem.h"
#include "Development/MiraiDeveloperSettings.h"
#include "GameMapsSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPlayerController)

namespace Mirai
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("MiraiPC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}

AMiraiPlayerController::AMiraiPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AMiraiPlayerCameraManager::StaticClass();

#if USING_CHEAT_MANAGER
	CheatClass = UMiraiCheatManager::StaticClass();
#endif // #if USING_CHEAT_MANAGER
}

void AMiraiPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMiraiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void AMiraiPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMiraiPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Disable replicating the PC target view as it doesn't work well for replays or client-side spectating.
	// The engine TargetViewRotation is only set in APlayerController::TickActor if the server knows ahead of time that 
	// a specific pawn is being spectated and it only replicates down for COND_OwnerOnly.
	// In client-saved replays, COND_OwnerOnly is never true and the target pawn is not always known at the time of recording.
	// To support client-saved replays, the replication of this was moved to ReplicatedViewRotation and updated in PlayerTick.
	DISABLE_REPLICATED_PROPERTY(APlayerController, TargetViewRotation);
}

void AMiraiPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void AMiraiPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// If we are auto running then add some player input
	if (GetIsAutoRunning())
	{
		if (APawn* CurrentPawn = GetPawn())
		{
			const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			CurrentPawn->AddMovementInput(MovementDirection, 1.0f);	
		}
	}

	AMiraiPlayerState* MiraiPlayerState = GetMiraiPlayerState();

	if (PlayerCameraManager && MiraiPlayerState)
	{
		APawn* TargetPawn = PlayerCameraManager->GetViewTargetPawn();

		if (TargetPawn)
		{
			// Update view rotation on the server so it replicates
			if (HasAuthority() || TargetPawn->IsLocallyControlled())
			{
				MiraiPlayerState->SetReplicatedViewRotation(TargetPawn->GetViewRotation());
			}

			// Update the target view rotation if the pawn isn't locally controlled
			if (!TargetPawn->IsLocallyControlled())
			{
				MiraiPlayerState = TargetPawn->GetPlayerState<AMiraiPlayerState>();
				if (MiraiPlayerState)
				{
					// Get it from the spectated pawn's player state, which may not be the same as the PC's playerstate
					TargetViewRotation = MiraiPlayerState->GetReplicatedViewRotation();
				}
			}
		}
	}
}

AMiraiPlayerState* AMiraiPlayerController::GetMiraiPlayerState() const
{
	return CastChecked<AMiraiPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UMiraiAbilitySystemComponent* AMiraiPlayerController::GetMiraiAbilitySystemComponent() const
{
	const AMiraiPlayerState* MiraiPS = GetMiraiPlayerState();
	return (MiraiPS ? MiraiPS->GetMiraiAbilitySystemComponent() : nullptr);
}

AMiraiHUD* AMiraiPlayerController::GetMiraiHUD() const
{
	return CastChecked<AMiraiHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

bool AMiraiPlayerController::TryToRecordClientReplay()
{
	// See if we should record a replay
	//if (ShouldRecordClientReplay())
	//{
	//	if (UMiraiReplaySubsystem* ReplaySubsystem = GetGameInstance()->GetSubsystem<UMiraiReplaySubsystem>())
	//	{
	//		APlayerController* FirstLocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	//		if (FirstLocalPlayerController == this)
	//		{
	//			// If this is the first player, update the spectator player for local replays and then record
	//			if (AMiraiGameState* GameState = Cast<AMiraiGameState>(GetWorld()->GetGameState()))
	//			{
	//				GameState->SetRecorderPlayerState(PlayerState);

	//				ReplaySubsystem->RecordClientReplay(this);
	//				return true;
	//			}
	//		}
	//	}
	//}
	return false;
}

//bool AMiraiPlayerController::ShouldRecordClientReplay()
//{
//	UWorld* World = GetWorld();
//	UGameInstance* GameInstance = GetGameInstance();
//	if (GameInstance != nullptr &&
//		World != nullptr &&
//		!World->IsPlayingReplay() &&
//		!World->IsRecordingClientReplay() &&
//		NM_DedicatedServer != GetNetMode() &&
//		IsLocalPlayerController())
//	{
//		FString DefaultMap = UGameMapsSettings::GetGameDefaultMap();
//		FString CurrentMap = World->URL.Map;
//
//#if WITH_EDITOR
//		CurrentMap = UWorld::StripPIEPrefixFromPackageName(CurrentMap, World->StreamingLevelsPrefix);
//#endif
//		if (CurrentMap == DefaultMap)
//		{
//			// Never record demos on the default frontend map, this could be replaced with a better check for being in the main menu
//			return false;
//		}
//
//		if (UReplaySubsystem* ReplaySubsystem = GameInstance->GetSubsystem<UReplaySubsystem>())
//		{
//			if (ReplaySubsystem->IsRecording() || ReplaySubsystem->IsPlaying())
//			{
//				// Only one at a time
//				return false;
//			}
//		}
//
//		// If this is possible, now check the settings
//		if (const UMiraiLocalPlayer* MiraiLocalPlayer = Cast<UMiraiLocalPlayer>(GetLocalPlayer()))
//		{
//			if (MiraiLocalPlayer->GetLocalSettings()->ShouldAutoRecordReplays())
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}

//void AMiraiPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
//{
//	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
//}

void AMiraiPlayerController::OnPlayerStateChanged()
{
	// Empty, place for derived classes to implement without having to hook all the other events
}

void AMiraiPlayerController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	// Unbind from the old player state, if any
	//FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	//if (LastSeenPlayerState != nullptr)
	//{
	//	if (IMiraiTeamAgentInterface* PlayerStateTeamInterface = Cast<IMiraiTeamAgentInterface>(LastSeenPlayerState))
	//	{
	//		OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
	//		PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
	//	}
	//}

	//// Bind to the new player state, if any
	//FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	//if (PlayerState != nullptr)
	//{
	//	if (IMiraiTeamAgentInterface* PlayerStateTeamInterface = Cast<IMiraiTeamAgentInterface>(PlayerState))
	//	{
	//		NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
	//		PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
	//	}
	//}

	//// Broadcast the team change (if it really has)
	//ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	//LastSeenPlayerState = PlayerState;
}

void AMiraiPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AMiraiPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AMiraiPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void AMiraiPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);

	if (const UMiraiLocalPlayer* MiraiLocalPlayer = Cast<UMiraiLocalPlayer>(InPlayer))
	{
		UMiraiSettingsShared* UserSettings = MiraiLocalPlayer->GetSharedSettings();
		UserSettings->OnSettingChanged.AddUObject(this, &ThisClass::OnSettingsChanged);

		OnSettingsChanged(UserSettings);
	}
}

void AMiraiPlayerController::OnSettingsChanged(UMiraiSettingsShared* InSettings)
{
	bForceFeedbackEnabled = InSettings->GetForceFeedbackEnabled();
}

//void AMiraiPlayerController::AddCheats(bool bForce)
//{
//#if USING_CHEAT_MANAGER
//	Super::AddCheats(true);
//#else //#if USING_CHEAT_MANAGER
//	Super::AddCheats(bForce);
//#endif // #else //#if USING_CHEAT_MANAGER
//}

//void AMiraiPlayerController::ServerCheat_Implementation(const FString& Msg)
//{
//#if USING_CHEAT_MANAGER
//	if (CheatManager)
//	{
//		UE_LOG(LogMirai, Warning, TEXT("ServerCheat: %s"), *Msg);
//		ClientMessage(ConsoleCommand(Msg));
//	}
//#endif // #if USING_CHEAT_MANAGER
//}

//bool AMiraiPlayerController::ServerCheat_Validate(const FString& Msg)
//{
//	return true;
//}

//void AMiraiPlayerController::ServerCheatAll_Implementation(const FString& Msg)
//{
//#if USING_CHEAT_MANAGER
//	if (CheatManager)
//	{
//		UE_LOG(LogMirai, Warning, TEXT("ServerCheatAll: %s"), *Msg);
//		for (TActorIterator<AMiraiPlayerController> It(GetWorld()); It; ++It)
//		{
//			AMiraiPlayerController* MiraiPC = (*It);
//			if (MiraiPC)
//			{
//				MiraiPC->ClientMessage(MiraiPC->ConsoleCommand(Msg));
//			}
//		}
//	}
//#endif // #if USING_CHEAT_MANAGER
//}

//bool AMiraiPlayerController::ServerCheatAll_Validate(const FString& Msg)
//{
//	return true;
//}

void AMiraiPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AMiraiPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		MiraiASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AMiraiPlayerController::OnCameraPenetratingTarget()
{
	bHideViewTargetPawnNextFrame = true;
}

void AMiraiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if WITH_SERVER_CODE && WITH_EDITOR
	if (GIsEditor && (InPawn != nullptr) && (GetPawn() == InPawn))
	{
		for (const FMiraiCheatToRun& CheatRow : GetDefault<UMiraiDeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnPlayerPawnPossession)
			{
				ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	SetIsAutoRunning(false);
}

void AMiraiPlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bIsAutoRunning = GetIsAutoRunning();
	if (bEnabled != bIsAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool AMiraiPlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	if (const UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		bIsAutoRunning = MiraiASC->GetTagCount(MiraiGameplayTags::Status_AutoRunning) > 0;
	}
	return bIsAutoRunning;
}

void AMiraiPlayerController::OnStartAutoRun()
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		MiraiASC->SetLooseGameplayTagCount(MiraiGameplayTags::Status_AutoRunning, 1);
		K2_OnStartAutoRun();
	}	
}

void AMiraiPlayerController::OnEndAutoRun()
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		MiraiASC->SetLooseGameplayTagCount(MiraiGameplayTags::Status_AutoRunning, 0);
		K2_OnEndAutoRun();
	}
}

void AMiraiPlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
		{
			const ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			if (Mirai::Input::ShouldAlwaysPlayForceFeedback || CurrentInputType == ECommonInputType::Gamepad || CurrentInputType == ECommonInputType::Touch)
			{
				InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
				return;
			}
		}
	}
	
	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void AMiraiPlayerController::UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents)
{
	Super::UpdateHiddenComponents(ViewLocation, OutHiddenComponents);

	if (bHideViewTargetPawnNextFrame)
	{
		AActor* const ViewTargetPawn = PlayerCameraManager ? Cast<AActor>(PlayerCameraManager->GetViewTarget()) : nullptr;
		if (ViewTargetPawn)
		{
			// internal helper func to hide all the components
			auto AddToHiddenComponents = [&OutHiddenComponents](const TInlineComponentArray<UPrimitiveComponent*>& InComponents)
			{
				// add every component and all attached children
				for (UPrimitiveComponent* Comp : InComponents)
				{
					if (Comp->IsRegistered())
					{
						OutHiddenComponents.Add(Comp->GetPrimitiveSceneId());

						for (USceneComponent* AttachedChild : Comp->GetAttachChildren())
						{
							static FName NAME_NoParentAutoHide(TEXT("NoParentAutoHide"));
							UPrimitiveComponent* AttachChildPC = Cast<UPrimitiveComponent>(AttachedChild);
							if (AttachChildPC && AttachChildPC->IsRegistered() && !AttachChildPC->ComponentTags.Contains(NAME_NoParentAutoHide))
							{
								OutHiddenComponents.Add(AttachChildPC->GetPrimitiveSceneId());
							}
						}
					}
				}
			};

			//TODO Solve with an interface.  Gather hidden components or something.
			//TODO Hiding isn't awesome, sometimes you want the effect of a fade out over a proximity, needs to bubble up to designers.

			// hide pawn's components
			TInlineComponentArray<UPrimitiveComponent*> PawnComponents;
			ViewTargetPawn->GetComponents(PawnComponents);
			AddToHiddenComponents(PawnComponents);

			//// hide weapon too
			//if (ViewTargetPawn->CurrentWeapon)
			//{
			//	TInlineComponentArray<UPrimitiveComponent*> WeaponComponents;
			//	ViewTargetPawn->CurrentWeapon->GetComponents(WeaponComponents);
			//	AddToHiddenComponents(WeaponComponents);
			//}
		}

		// we consumed it, reset for next frame
		bHideViewTargetPawnNextFrame = false;
	}
}

//void AMiraiPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
//{
//	UE_LOG(LogMiraiTeams, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
//}

//FGenericTeamId AMiraiPlayerController::GetGenericTeamId() const
//{
//	if (const IMiraiTeamAgentInterface* PSWithTeamInterface = Cast<IMiraiTeamAgentInterface>(PlayerState))
//	{
//		return PSWithTeamInterface->GetGenericTeamId();
//	}
//	return FGenericTeamId::NoTeam;
//}

//FOnMiraiTeamIndexChangedDelegate* AMiraiPlayerController::GetOnTeamIndexChangedDelegate()
//{
//	return &OnTeamChangedDelegate;
//}

void AMiraiPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

//////////////////////////////////////////////////////////////////////
// AMiraiReplayPlayerController

void AMiraiReplayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// The state may go invalid at any time due to scrubbing during a replay
	if (!IsValid(FollowedPlayerState))
	{
		UWorld* World = GetWorld();

		// Listen for changes for both recording and playback
		//if (AMiraiGameState* GameState = Cast<AMiraiGameState>(World->GetGameState()))
		//{
		//	if (!GameState->OnRecorderPlayerStateChangedEvent.IsBoundToObject(this))
		//	{
		//		GameState->OnRecorderPlayerStateChangedEvent.AddUObject(this, &ThisClass::RecorderPlayerStateUpdated);
		//	}
		//	if (APlayerState* RecorderState = GameState->GetRecorderPlayerState())
		//	{
		//		RecorderPlayerStateUpdated(RecorderState);
		//	}
		//}
	}
}

//void AMiraiReplayPlayerController::SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds)
//{
//	// Default behavior is to interpolate to TargetViewRotation which is set from APlayerController::TickActor but it's not very smooth
//
//	Super::SmoothTargetViewRotation(TargetPawn, DeltaSeconds);
//}

//bool AMiraiReplayPlayerController::ShouldRecordClientReplay()
//{
//	return false;
//}

void AMiraiReplayPlayerController::RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState)
{
	if (NewRecorderPlayerState)
	{
		FollowedPlayerState = NewRecorderPlayerState;

		// Bind to when pawn changes and call now
		NewRecorderPlayerState->OnPawnSet.AddUniqueDynamic(this, &AMiraiReplayPlayerController::OnPlayerStatePawnSet);
		OnPlayerStatePawnSet(NewRecorderPlayerState, NewRecorderPlayerState->GetPawn(), nullptr);
	}
}

void AMiraiReplayPlayerController::OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn)
{
	if (ChangedPlayerState == FollowedPlayerState)
	{
		SetViewTarget(NewPlayerPawn);
	}
}

