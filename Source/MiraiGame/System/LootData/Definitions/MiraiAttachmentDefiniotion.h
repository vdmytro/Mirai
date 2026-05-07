// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/LootData/Definitions/MiraiLootDefinition.h"
#include "MiraiAttachmentDefiniotion.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MIRAIGAME_API UMiraiAttachmentDefiniotion : public UMiraiLootDefinition
{
	GENERATED_BODY()
	
public:
    //Skeletal mesh of the attachment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Visual")
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    //Attachment type (Base / Functional / Additional )
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
    EAttachmentType AttachmentType;

    //List of compatible attachments this attachment can be attached to
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Compatibility")
    TSet<TSoftObjectPtr<UMiraiAttachmentDefiniotion>> CompatibleParents;

    //List of sockets where this attachment can be attached
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Sockets")
    TSet<FName> CompatibleSockets;

    //List of sockets of this attachment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Sockets")
    TSet<FName> Sockets;
	

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("AttachmentLoot", GetFName());
    }
};
