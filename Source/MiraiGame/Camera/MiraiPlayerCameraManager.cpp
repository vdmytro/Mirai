// Copyright 

#include "MiraiPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "MiraiCameraComponent.h"
#include "MiraiUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AMiraiPlayerCameraManager::AMiraiPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = Mirai_CAMERA_DEFAULT_FOV;
	ViewPitchMin = Mirai_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = Mirai_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UMiraiUICameraManagerComponent>(UICameraComponentName);
}

UMiraiUICameraManagerComponent* AMiraiPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AMiraiPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AMiraiPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("MiraiPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UMiraiCameraComponent* CameraComponent = UMiraiCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}

