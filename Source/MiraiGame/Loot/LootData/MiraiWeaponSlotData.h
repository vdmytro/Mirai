// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "MiraiWeaponSlotData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MIRAIGAME_API FMiraiWeaponSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SlotId;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTagContainer AcceptedTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTagContainer BlockedTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName AttachSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTransform RelativeTransform = FTransform::Identity;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bRequired = false;
};
