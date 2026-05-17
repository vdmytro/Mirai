// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"

#include "MiraiAmmoDefinition.generated.h"

class UNiagaraSystem;

/* ================================
 * VFX
 * ================================ */

USTRUCT(BlueprintType)
struct FAmmoVFX
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UNiagaraSystem> Impact;

};

/* ================================
 * Audio
 * ================================ */

USTRUCT(BlueprintType)
struct FAmmoSounds
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Impact;

};


/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiAmmoDefinition : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:

	//Bullet StaticMesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Visual")
    TObjectPtr<UStaticMesh> BulletMesh;

	//Sleeve StaticMesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Visual")
    TObjectPtr<UStaticMesh> SleeveMesh;

	//Cartridge StaticMesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Visual")
	TObjectPtr<UStaticMesh> CartridgeMesh;

	//AmmoType
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
    EAmmoType AmmoType;

	//DecalsList
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Visual|Decals")
    TSoftObjectPtr<UTexture2DArray> MetalDecals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Visual|Decals")
    TSoftObjectPtr<UTexture2DArray> WoodDecals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Visual|Decals")
    TSoftObjectPtr<UTexture2DArray> DirtDecals;

	/* ================== VFX ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|VFX")
    FAmmoVFX VFX;

    /* ================== Sounds ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo|Audio")
    FAmmoSounds Sounds;


	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("AmmoLoot", GetFName());
	}
	
};
