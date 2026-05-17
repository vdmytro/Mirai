// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DebugCameraController.h"

#include "MiraiDebugCameraController.generated.h"

class UObject;


/**
 * AMiraiDebugCameraController
 *
 *	Used for controlling the debug camera when it is enabled via the cheat manager.
 */
UCLASS()
class AMiraiDebugCameraController : public ADebugCameraController
{
	GENERATED_BODY()

public:

	AMiraiDebugCameraController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void AddCheats(bool bForce) override;
};
