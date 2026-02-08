// Copyright (c) 2025, dvolkov. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "MiraiLootTypes.generated.h"

/*
 * ============================
 * Loot Core
 * ============================
 */

UENUM(BlueprintType)
enum class ELootType : uint8
{
    None        UMETA(DisplayName = "None"), //(for undefined or broken values)
    Weapon      UMETA(DisplayName = "Weapon"),
    Attachment  UMETA(DisplayName = "Attachment"),
    Equipment   UMETA(DisplayName = "Equipment"), //(Grenade, ...)
    Gear        UMETA(DisplayName = "Gear"), //(Wear, Backpacks, tactical bags, ... )
    Ammo        UMETA(DisplayName = "Ammo"),
    QuestItem   UMETA(DisplayName = "QuestItem"),
    Trash       UMETA(DisplayName = "Trash"),
};

/*
 * ============================
 * Weapon
 * ============================
 */

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Rifle   UMETA(DisplayName = "Rifle"),
    SMG     UMETA(DisplayName = "SMG"),
    Shotgun UMETA(DisplayName = "Shotgun"),
    Sniper  UMETA(DisplayName = "Sniper"),
    Pistol  UMETA(DisplayName = "Pistol"),
    Melee   UMETA(DisplayName = "Melee"),
};

/*
 * ============================
 * Attachment
 * ============================
 */

UENUM(BlueprintType)
enum class EAttachmentType : uint8
{
    None        UMETA(DisplayName = "None"), //(for undefined or broken values)
    Base        UMETA(DisplayName = "Base"),
    Functional  UMETA(DisplayName = "Functional"),
    Additional  UMETA(DisplayName = "Additional"),
};

/*
 * ============================
 * Ammo
 * ============================
 */

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
    None    UMETA(DisplayName = "None"), //(for undefined or broken values)
    OTGSH   UMETA(DisplayName = "12 Gauge Shot"), //(12 gauge shot)
    OTGSL   UMETA(DisplayName = "12 Gauge Slugs"), //(12 gauge slugs)
    TZG     UMETA(DisplayName = "20 Gauge"), //(20 gauge)
    TTXSF   UMETA(DisplayName = "23x75 mm"), //(23x75 mm)
    NXOE    UMETA(DisplayName = "9x18 mm"), //(9x18 mm)
    SSTXTF  UMETA(DisplayName = "7.62x25 mm"), //(7.62x25 mm)
    NXON    UMETA(DisplayName = "9x19 mm"), //(9x19 mm)
    DFF     UMETA(DisplayName = ".45"), //(.45)
    DFZ     UMETA(DisplayName = ".50"), //(.50)
    NXTO    UMETA(DisplayName = "9x21 mm"), //(9x21 mm)
    DTFS    UMETA(DisplayName = ".357 Magnum"), //(.357 magnum)
    FSXTE   UMETA(DisplayName = "5.7x28 mm"), //(5.7x28 mm)
    FSXTZ   UMETA(DisplayName = "4.6x30 mm"), //(4.6x30 mm)
    NXTN    UMETA(DisplayName = "9x39 mm"), //(9x39 mm)
    DTSS    UMETA(DisplayName = ".366"), //(.366)
    FFFXTN  UMETA(DisplayName = "5.45x39 mm"), //(5.45x39 mm)
    FFSXFF  UMETA(DisplayName = "5.56x45 mm"), //(5.56x45 mm)
    SSTXTN  UMETA(DisplayName = "7.62x39 mm"), //(7.62x39 mm)
    DTZZBLK UMETA(DisplayName = ".300 blk"), //(.300 blk)
    SEXFO   UMETA(DisplayName = "6.8x51mm"), //(6.8x51mm)
    SSTXFO  UMETA(DisplayName = "7.62x51 mm"), //(7.62x51 mm)
    SSTXFFR UMETA(DisplayName = "7.62x54r"), //(7.62x54r)
    OTXFF   UMETA(DisplayName = "12.7x55 mm"), //(12.7x55 mm)
    DTTELM  UMETA(DisplayName = ".338 Lapua Magnum"), //(.338 lapua magnum)
    DFZBMG  UMETA(DisplayName = ".50 bmg"), //(.50 bmg)
    DTZEME  UMETA(DisplayName = ".308 marlin express"), //(.308 marlin express)
    NTXSF   UMETA(DisplayName = "9.3x64 mm"), //(9.3x64 mm)
    MWA      UMETA(DisplayName = "Mounted Weapons Ammo"), //(mounted weapons)
    OTH     UMETA(DisplayName = "Other"), //(other)
};

/*
 * ============================
 * Interaction
 * ============================
 */

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    None        UMETA(DisplayName = "None"), //(for undefined or broken values)
    Quest       UMETA(DisplayName = "Quest"),
    Trash       UMETA(DisplayName = "Trash"),
};
