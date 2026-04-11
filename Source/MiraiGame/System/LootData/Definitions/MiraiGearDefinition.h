// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"

#include "MiraiGearDefinition.generated.h"

/* ================================
 * Audio
 * ================================ */

USTRUCT(BlueprintType)
struct FGearSounds
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Equip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Unequip;
};

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiGearDefinition : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:

	//SkeletalMesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear|Visual")
    TObjectPtr<USkeletalMesh> SkeletalMesh;

	//InventoryData
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear|Stats")
    FDataTableRowHandle BaseStats;

	/* ================== Sounds ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear|Audio")
    FGearSounds Sounds;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GearLoot", GetFName());
	}
	
};
