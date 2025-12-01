// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "MiraiLobbyBackground.generated.h"

class UObject;
class UWorld;

/**
 * Developer settings / editor cheats
 */
UCLASS(config=EditorPerProjectUserSettings, MinimalAPI)
class UMiraiLobbyBackground : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> BackgroundLevel;
};
