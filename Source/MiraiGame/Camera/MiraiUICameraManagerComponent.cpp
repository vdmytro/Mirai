// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiUICameraManagerComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "MiraiPlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UMiraiUICameraManagerComponent* UMiraiUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (AMiraiPlayerCameraManager* PCCamera = Cast<AMiraiPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UMiraiUICameraManagerComponent::UMiraiUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UMiraiUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UMiraiUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<AMiraiPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UMiraiUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UMiraiUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UMiraiUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
