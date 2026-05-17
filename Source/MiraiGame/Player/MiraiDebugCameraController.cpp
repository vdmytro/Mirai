// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiDebugCameraController.h"
#include "MiraiCheatManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiDebugCameraController)


AMiraiDebugCameraController::AMiraiDebugCameraController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use the same cheat class as MiraiPlayerController to allow toggling the debug camera through cheats.
	CheatClass = UMiraiCheatManager::StaticClass();
}

void AMiraiDebugCameraController::AddCheats(bool bForce)
{
	// Mirrors MiraiPlayerController's AddCheats() to avoid the player becoming stuck in the debug camera.
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else //#if USING_CHEAT_MANAGER
	Super::AddCheats(bForce);
#endif // #else //#if USING_CHEAT_MANAGER
}

