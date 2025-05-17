// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Character/MiraiCharacterMovementComponent.h"

#include "TopDownArenaMovementComponent.generated.h"

class UObject;

UCLASS()
class UTopDownArenaMovementComponent : public UMiraiCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UTopDownArenaMovementComponent(const FObjectInitializer& ObjectInitializer);

	//~UMovementComponent interface
	virtual float GetMaxSpeed() const override;
	//~End of UMovementComponent interface

};
