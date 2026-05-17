// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiGameData.h"
#include "MiraiAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiGameData)

UMiraiGameData::UMiraiGameData()
{
}

const UMiraiGameData& UMiraiGameData::UMiraiGameData::Get()
{
	return UMiraiAssetManager::Get().GetGameData();
}
