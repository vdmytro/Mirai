// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPawnData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPawnData)

UMiraiPawnData::UMiraiPawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	DefaultCameraMode = nullptr;
}

