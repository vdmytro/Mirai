// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"
#include "MiraiEquipmentDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiEquipmentDefinition : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EquipmentLoot", GetFName());
	}
	
	
};
