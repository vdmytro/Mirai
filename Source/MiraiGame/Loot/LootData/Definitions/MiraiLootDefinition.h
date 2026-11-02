// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Loot/LootData/MiraiLootTypes.h"

#include "MiraiLootDefinition.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class MIRAIGAME_API UMiraiLootDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // Loot Type
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
    ELootType LootType;

    // Name
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    FText DisplayName;

    // Description
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (MultiLine = true))
    FText Description;

    // Icon
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSoftObjectPtr<UTexture2D> Icon;

    // Tags
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FGameplayTagContainer Tags;

    //Weight
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0"))
    float Weight = 0.f;


    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("BaseLoot", GetFName());
    }
};
