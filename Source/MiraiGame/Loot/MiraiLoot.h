// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiraiLoot.generated.h"


UCLASS()
class MIRAIGAME_API AMiraiLoot : public AActor
{
	GENERATED_BODY()
	
public:	
	AMiraiLoot();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

};
