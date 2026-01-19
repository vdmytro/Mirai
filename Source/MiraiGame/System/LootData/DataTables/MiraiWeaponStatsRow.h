// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "MiraiWeaponStatsRow.generated.h"

/**
 * 
 * 
 */
USTRUCT(BlueprintType)
struct FMiraiWeaponStatsRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
    float Damage = 0.0f; //25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fire")
    float FireRate = 0.0f; //5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
    int32 MagazineSize = 0.0f; //30;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
    float ReloadTime = 0.0f; //2.2f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Handling")
    float Recoil = 0.0f; //1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
    float EffectiveRange = 0.0f; //1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
    FGameplayTagContainer WeaponTags;
};