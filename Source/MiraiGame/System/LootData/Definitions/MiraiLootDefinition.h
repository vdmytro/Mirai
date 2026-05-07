// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "System/LootData/MiraiLootTypes.h"

#include "MiraiLootDefinition.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class MIRAIGAME_API UMiraiLootDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    //Loot Type
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
    ELootType LootType;

    //Name
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    FText Name;

    //Description
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (MultiLine = true))
    FText Description;

    //Icon
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSoftObjectPtr<UTexture2D> Icon;

    //Weight
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0"))
    float Weight = 1.f;


    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("BaseLoot", GetFName());
    }
};
