// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModularPawn.h"

#include "MiraiPawn.generated.h"

class AController;
class UObject;
struct FFrame;

/**
 * AMiraiPawn
 */
UCLASS()
class MIRAIGAME_API AMiraiPawn : public AModularPawn
{
	GENERATED_BODY()

	AMiraiPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

// AMiraiPawn
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

// AActor
public: 
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


// APawn
public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
};
