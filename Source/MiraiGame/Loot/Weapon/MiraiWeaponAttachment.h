// Copyright (c) 2025, dvolkov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot/MiraiLoot.h"

#include "MiraiWeaponAttachment.generated.h"

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

	

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
};
