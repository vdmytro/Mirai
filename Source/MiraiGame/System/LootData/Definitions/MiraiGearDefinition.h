// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"
#include "MiraiGearDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiGearDefinition : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GearLoot", GetFName());
	}
	
	
};
