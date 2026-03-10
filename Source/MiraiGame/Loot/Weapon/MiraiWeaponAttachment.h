// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot/MiraiLoot.h"
      
#include "MiraiWeaponAttachment.generated.h"

class UMiraiAttachmentDefiniotion;

UCLASS()
class MIRAIGAME_API AMiraiWeaponAttachment : public AMiraiLoot
{
	GENERATED_BODY()
	
public:	
	AMiraiWeaponAttachment();
protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FString GetName();

private:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY( BlueprintReadWrite, VisibleInstanceOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMiraiAttachmentDefiniotion> DataAsset;
};
