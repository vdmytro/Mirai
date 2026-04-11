// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"

#include "MiraiEquipmentDefinition.generated.h"

class UNiagaraSystem;

/* ================================
 * Animation Set
 * ================================ */

USTRUCT(BlueprintType)
struct FEquipmentAnimationSet
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Action;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Equip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Unequip;
};

/* ================================
 * VFX
 * ================================ */

USTRUCT(BlueprintType)
struct FEquipmentVFX
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UNiagaraSystem> Flash;

};

/* ================================
 * Audio
 * ================================ */

USTRUCT(BlueprintType)
struct FEquipmentSounds
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Action;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Equip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Unequip;
};

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiEquipmentDefinition : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:

	//Skeletal mesh of the Equipment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Visual")
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    /* ================== Stats ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Stats")
    FDataTableRowHandle BaseStats;

	/* ================== Animations ================== */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Animation")
    FEquipmentAnimationSet EquipmentAnimations;

	/* ================== VFX ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|VFX")
    FEquipmentVFX VFX;

    /* ================== Sounds ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Audio")
    FEquipmentSounds Sounds;


	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EquipmentLoot", GetFName());
	}
	
	
};
