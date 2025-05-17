// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Player/MiraiPlayerSpawningManagerComponent.h"

#include "TDM_PlayerSpawningManagmentComponent.generated.h"

class AActor;
class AController;
class AMiraiPlayerStart;
class UObject;

/**
 * 
 */
UCLASS()
class UTDM_PlayerSpawningManagmentComponent : public UMiraiPlayerSpawningManagerComponent
{
	GENERATED_BODY()

public:

	UTDM_PlayerSpawningManagmentComponent(const FObjectInitializer& ObjectInitializer);

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AMiraiPlayerStart*>& PlayerStarts) override;
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) override;

protected:

};
