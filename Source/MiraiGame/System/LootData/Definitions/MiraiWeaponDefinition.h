// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"
#include "MiraiWeaponDefinition.generated.h"

class UMiraiAttachmentDefinition;
class UMiraiAmmoDefinition;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FWeaponAttachmentSlot
{
    GENERATED_BODY()

    UPROPERTY()
    FName Socket;

    UPROPERTY()
    TSoftObjectPtr<UMiraiAttachmentDefinition> Attachment;

};

uint32 GetTypeHash(const FWeaponAttachmentSlot& obj);

/* ================================
 * Animation Set
 * ================================ */

USTRUCT(BlueprintType)
struct FWeaponAnimationSet
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Fire;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Reload;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Equip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UAnimSequence> Unequip;
};

/* ================================
 * VFX
 * ================================ */

USTRUCT(BlueprintType)
struct FWeaponVFX
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UNiagaraSystem> Flash;

};

/* ================================
 * Audio
 * ================================ */

USTRUCT(BlueprintType)
struct FWeaponSounds
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Fire;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Reload;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Equip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<USoundBase> Unequip;
};

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiWeaponDefinition : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:
	/* ================== Attachments ================== */
    //Skeletal mesh of the attachment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attachments")
    TObjectPtr<USkeletalMesh> BaseSkeletalMesh;

    // Default Base-Type Attachments (spawned by default)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attachments")
    TSet<FWeaponAttachmentSlot> DefaultBaseAttachments;

    // All compatible Base-Type Attachments
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attachments")
    TSet<TSoftObjectPtr<UMiraiAttachmentDefinition>> CompatibleAttachments;

    /* ================== Weapon Params ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
    EAmmoType AmmoType;

    // Sub-types of ammo
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
    TSet<TSoftObjectPtr<UMiraiAmmoDefinition>> AmmoSubTypes;

    /* ================== Stats ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
    FDataTableRowHandle BaseStats;

    /* ================== Animations ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
    FWeaponAnimationSet Animations;

    /* ================== VFX ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX")
    FWeaponVFX VFX;

    /* ================== Sounds ================== */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Audio")
    FWeaponSounds Sounds;
	
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("WeaponLoot", GetFName());
    }
};
