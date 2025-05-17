// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiCheatManager.h"
#include "GameFramework/Pawn.h"
#include "MiraiPlayerController.h"
#include "MiraiDebugCameraController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Console.h"
#include "GameFramework/HUD.h"
#include "System/MiraiAssetManager.h"
#include "System/MiraiGameData.h"
#include "MiraiGameplayTags.h"
#include "AbilitySystem/MiraiAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
//#include "Character/MiraiHealthComponent.h"
#include "Character/MiraiPawnExtensionComponent.h"
//#include "System/MiraiSystemStatics.h"
#include "Development/MiraiDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiCheatManager)

DEFINE_LOG_CATEGORY(LogMiraiCheat);

namespace MiraiCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("MiraiCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("MiraiCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
};


UMiraiCheatManager::UMiraiCheatManager()
{
	DebugCameraControllerClass = AMiraiDebugCameraController::StaticClass();
}

void UMiraiCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

#if WITH_EDITOR
	if (GIsEditor)
	{
		APlayerController* PC = GetOuterAPlayerController();
		for (const FMiraiCheatToRun& CheatRow : GetDefault<UMiraiDeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnCheatManagerCreated)
			{
				PC->ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	if (MiraiCheat::bStartInGodMode)
	{
		God();	
	}
}

void UMiraiCheatManager::CheatOutputText(const FString& TextToOutput)
{
#if USING_CHEAT_MANAGER
	// Output to the console.
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	// Output to log.
	UE_LOG(LogMiraiCheat, Display, TEXT("%s"), *TextToOutput);
#endif // USING_CHEAT_MANAGER
}

void UMiraiCheatManager::Cheat(const FString& Msg)
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
		//MiraiPC->ServerCheat(Msg.Left(128));
	}
}

void UMiraiCheatManager::CheatAll(const FString& Msg)
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
		//MiraiPC->ServerCheatAll(Msg.Left(128));
	}
}

void UMiraiCheatManager::PlayNextGame()
{
	//UMiraiSystemStatics::PlayNextGame(this);
}

void UMiraiCheatManager::EnableDebugCamera()
{
	Super::EnableDebugCamera();
}

void UMiraiCheatManager::DisableDebugCamera()
{
	FVector DebugCameraLocation;
	FRotator DebugCameraRotation;

	ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* OriginalPC = nullptr;

	if (DebugCC)
	{
		OriginalPC = DebugCC->OriginalControllerRef;
		DebugCC->GetPlayerViewPoint(DebugCameraLocation, DebugCameraRotation);
	}

	Super::DisableDebugCamera();

	if (OriginalPC && OriginalPC->PlayerCameraManager && (OriginalPC->PlayerCameraManager->CameraStyle == MiraiCheat::NAME_Fixed))
	{
		OriginalPC->SetInitialLocationAndRotation(DebugCameraLocation, DebugCameraRotation);

		OriginalPC->PlayerCameraManager->ViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->ViewTarget.POV.Rotation = DebugCameraRotation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Rotation = DebugCameraRotation;
	}
}

bool UMiraiCheatManager::InDebugCamera() const
{
	return (Cast<ADebugCameraController>(GetOuter()) ? true : false);
}

void UMiraiCheatManager::EnableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(MiraiCheat::NAME_Fixed);
	}
}

void UMiraiCheatManager::DisableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(NAME_Default);
	}
}

bool UMiraiCheatManager::InFixedCamera() const
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	const APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		return (PC->PlayerCameraManager->CameraStyle == MiraiCheat::NAME_Fixed);
	}

	return false;
}

void UMiraiCheatManager::ToggleFixedCamera()
{
	if (InFixedCamera())
	{
		DisableFixedCamera();
	}
	else
	{
		EnableFixedCamera();
	}
}

void UMiraiCheatManager::CycleDebugCameras()
{
	if (!MiraiCheat::bEnableDebugCameraCycling)
	{
		return;
	}
	
	if (InDebugCamera())
	{
		EnableFixedCamera();
		DisableDebugCamera();
	}
	else if (InFixedCamera())
	{
		DisableFixedCamera();
		DisableDebugCamera();
	}
	else
	{
		EnableDebugCamera();
		DisableFixedCamera();
	}
}

void UMiraiCheatManager::CycleAbilitySystemDebug()
{
	APlayerController* PC = Cast<APlayerController>(GetOuterAPlayerController());

	if (PC && PC->MyHUD)
	{
		if (!PC->MyHUD->bShowDebugInfo || !PC->MyHUD->DebugDisplay.Contains(TEXT("AbilitySystem")))
		{
			PC->MyHUD->ShowDebug(TEXT("AbilitySystem"));
		}

		PC->ConsoleCommand(TEXT("AbilitySystem.Debug.NextCategory"));
	}
}

void UMiraiCheatManager::CancelActivatedAbilities()
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetPlayerAbilitySystemComponent())
	{
		const bool bReplicateCancelAbility = true;
		MiraiASC->CancelInputActivatedAbilities(bReplicateCancelAbility);
	}
}

void UMiraiCheatManager::AddTagToSelf(FString TagName)
{
	FGameplayTag Tag = MiraiGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UMiraiAbilitySystemComponent* MiraiASC = GetPlayerAbilitySystemComponent())
		{
			MiraiASC->AddDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogMiraiCheat, Display, TEXT("AddTagToSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UMiraiCheatManager::RemoveTagFromSelf(FString TagName)
{
	FGameplayTag Tag = MiraiGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UMiraiAbilitySystemComponent* MiraiASC = GetPlayerAbilitySystemComponent())
		{
			MiraiASC->RemoveDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogMiraiCheat, Display, TEXT("RemoveTagFromSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UMiraiCheatManager::DamageSelf(float DamageAmount)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerDamage(MiraiASC, DamageAmount);
	}
}

void UMiraiCheatManager::DamageTarget(float DamageAmount)
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
		if (MiraiPC->GetNetMode() == NM_Client)
		{
			//// Automatically send cheat to server for convenience.
			//MiraiPC->ServerCheat(FString::Printf(TEXT("DamageTarget %.2f"), DamageAmount));
			return;
		}

		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(MiraiPC, TargetHitResult);

		if (UMiraiAbilitySystemComponent* MiraiTargetASC = Cast<UMiraiAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerDamage(MiraiTargetASC, DamageAmount);
		}
	}
}

void UMiraiCheatManager::ApplySetByCallerDamage(UMiraiAbilitySystemComponent* MiraiASC, float DamageAmount)
{
	check(MiraiASC);

	TSubclassOf<UGameplayEffect> DamageGE = UMiraiAssetManager::GetSubclass(UMiraiGameData::Get().DamageGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = MiraiASC->MakeOutgoingSpec(DamageGE, 1.0f, MiraiASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(MiraiGameplayTags::SetByCaller_Damage, DamageAmount);
		MiraiASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UMiraiCheatManager::HealSelf(float HealAmount)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerHeal(MiraiASC, HealAmount);
	}
}

void UMiraiCheatManager::HealTarget(float HealAmount)
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(MiraiPC, TargetHitResult);

		if (UMiraiAbilitySystemComponent* MiraiTargetASC = Cast<UMiraiAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerHeal(MiraiTargetASC, HealAmount);
		}
	}
}

void UMiraiCheatManager::ApplySetByCallerHeal(UMiraiAbilitySystemComponent* MiraiASC, float HealAmount)
{
	check(MiraiASC);

	TSubclassOf<UGameplayEffect> HealGE = UMiraiAssetManager::GetSubclass(UMiraiGameData::Get().HealGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = MiraiASC->MakeOutgoingSpec(HealGE, 1.0f, MiraiASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(MiraiGameplayTags::SetByCaller_Heal, HealAmount);
		MiraiASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UMiraiAbilitySystemComponent* UMiraiCheatManager::GetPlayerAbilitySystemComponent() const
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
		return MiraiPC->GetMiraiAbilitySystemComponent();
	}
	return nullptr;
}

void UMiraiCheatManager::DamageSelfDestruct()
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
 		if (const UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(MiraiPC->GetPawn()))
		{
			if (PawnExtComp->HasReachedInitState(MiraiGameplayTags::InitState_GameplayReady))
			{
				//if (UMiraiHealthComponent* HealthComponent = UMiraiHealthComponent::FindHealthComponent(MiraiPC->GetPawn()))
				//{
				//	HealthComponent->DamageSelfDestruct();
				//}
			}
		}
	}
}

void UMiraiCheatManager::God()
{
	if (AMiraiPlayerController* MiraiPC = Cast<AMiraiPlayerController>(GetOuterAPlayerController()))
	{
		if (MiraiPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			//MiraiPC->ServerCheat(FString::Printf(TEXT("God")));
			return;
		}

		if (UMiraiAbilitySystemComponent* MiraiASC = MiraiPC->GetMiraiAbilitySystemComponent())
		{
			const FGameplayTag Tag = MiraiGameplayTags::Cheat_GodMode;
			const bool bHasTag = MiraiASC->HasMatchingGameplayTag(Tag);

			if (bHasTag)
			{
				MiraiASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				MiraiASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

void UMiraiCheatManager::UnlimitedHealth(int32 Enabled)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetPlayerAbilitySystemComponent())
	{
		const FGameplayTag Tag = MiraiGameplayTags::Cheat_UnlimitedHealth;
		const bool bHasTag = MiraiASC->HasMatchingGameplayTag(Tag);

		if ((Enabled == -1) || ((Enabled > 0) && !bHasTag) || ((Enabled == 0) && bHasTag))
		{
			if (bHasTag)
			{
				MiraiASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				MiraiASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

