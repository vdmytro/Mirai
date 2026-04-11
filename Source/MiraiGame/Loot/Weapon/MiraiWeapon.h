// Copyright (c) 2025, dvolkov. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot/MiraiLoot.h"

#include "MiraiWeapon.generated.h"

class UMiraiWeaponDefinition;
class UMiraiAttachmentDefiniotion;

USTRUCT(BlueprintType)
struct FAttachmentSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName; //Socket name that current node attached to

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMiraiAttachmentDefiniotion> DataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Parentindex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;
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

protected:
	virtual void BeginPlay() override;

	//Properties
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", Meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UMiraiWeaponDefinition> DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data", Meta = (AllowPrivateAccess = "true"))
	TArray<FAttachmentSlot> AttachmentDataList;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

};
