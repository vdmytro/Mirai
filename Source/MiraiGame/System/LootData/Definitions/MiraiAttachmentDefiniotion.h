// Copyright (c) 2025, dvolkov. All rights reserved.

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
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

    //Attachment type (Base / Functional / Additional )
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
    EAttachmentType AttachmentType;

    //List of compatible attachments this attachment can be attached to
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Compatibility")
    TArray<TSoftObjectPtr<UMiraiAttachmentDefiniotion>> CompatibleParents;

    //List of sockets where this attachment can be attached
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment|Sockets")
    TArray<FName> CompatibleSockets;
	

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("AttachmentLoot", GetFName());
    }
};
