// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot/MiraiLoot.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "MiraiWeapon.generated.h"

class UMiraiWeaponDefinition;
class UMiraiAttachmentDefinition;
struct FAttachmentSlotArray;

USTRUCT(BlueprintType)
struct FAttachmentSlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName; //Socket name that current node attached to

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMiraiAttachmentDefinition> DataAsset = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;

	// Overriding PreReplicatedRemove and PostReplicatedAdd (optional)
	void PreReplicatedRemove(const FAttachmentSlotArray& InArraySerializer) {}
	void PostReplicatedAdd(const FAttachmentSlotArray& InArraySerializer) {}
	void PostReplicatedChange(const FAttachmentSlotArray& InArraySerializer) {}
};

USTRUCT(BlueprintType)
struct FAttachmentSlotArray : public FFastArraySerializer
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FAttachmentSlot> AttachmentDataList;

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FAttachmentSlot, FAttachmentSlotArray>(AttachmentDataList, DeltaParams, *this);
    }
};

UCLASS()
class MIRAIGAME_API AMiraiWeapon : public AMiraiLoot
{
	GENERATED_BODY()
	
public:	
	AMiraiWeapon();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor)
	virtual void RebuildMeshes();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RebuildMeshes();
	void Server_RebuildMeshes_Implementation();
	bool Server_RebuildMeshes_Validate();

protected:
	virtual void BeginPlay() override;

	//Properties
private:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Data", Meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UMiraiWeaponDefinition> DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DataArray, Category = "Data", Meta = (AllowPrivateAccess = "true"))
	FAttachmentSlotArray DataArray;

	UFUNCTION()
	void OnRep_DataArray();

};
