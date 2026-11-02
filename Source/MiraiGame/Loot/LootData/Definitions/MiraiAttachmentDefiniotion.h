// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Loot/LootData/Definitions/MiraiLootDefinition.h"
#include "Loot/LootData/MiraiWeaponSlotData.h"

#include "MiraiAttachmentDefiniotion.generated.h"

UENUM(BlueprintType)
enum class EWeaponPresentationType : uint8
{
    StaticMesh,
    SkeletalMesh
};

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiAttachmentDefiniotion : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Visual")
    EWeaponPresentationType PresentationType = EWeaponPresentationType::StaticMesh;

    //Skeletal mesh of the attachment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Visual", meta = (EditCondition = "PresentationType == EWeaponPresentationType::StaticMesh"))
    TObjectPtr<UStaticMesh> StaticMesh;

    //Skeletal mesh of the attachment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Visual", meta = (EditCondition = "PresentationType == EWeaponPresentationType::SkeletalMesh"))
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Slots")
    TArray<FMiraiWeaponSlotData> Slots;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Stats")
    //FWeaponStatModifiers StatModifiers;

    //Attachment type (Base / Functional / Additional )
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
    EAttachmentType AttachmentType;

    //List of compatible attachments this attachment can be attached to
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Compatibility")
    TSet<TSoftObjectPtr<UMiraiAttachmentDefiniotion>> CompatibleParents;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Compatibility")
    FGameplayTagContainer CompatiblePlatforms;

    //List of sockets of this attachment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Sockets")
    TSet<FName> Sockets;
	

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("AttachmentLoot", GetFName());
    }
};
