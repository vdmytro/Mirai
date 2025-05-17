// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/MiraiCameraMode.h"
#include "Curves/CurveFloat.h"

#include "MiraiCameraMode_TopDownArenaCamera.generated.h"

class UObject;


/**
 * UMiraiCameraMode_TopDownArenaCamera
 *
 *	A basic third person camera mode that looks down at a fixed arena.
 */
UCLASS(Abstract, Blueprintable)
class UMiraiCameraMode_TopDownArenaCamera : public UMiraiCameraMode
{
	GENERATED_BODY()

public:

	UMiraiCameraMode_TopDownArenaCamera();

protected:

	//~UMiraiCameraMode interface
	virtual void UpdateView(float DeltaTime) override;
	//~End of UMiraiCameraMode interface

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	float ArenaWidth;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	float ArenaHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FRotator DefaultPivotRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FRuntimeFloatCurve BoundsSizeToDistance;
};
