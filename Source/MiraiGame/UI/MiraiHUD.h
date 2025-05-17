// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"

#include "MiraiHUD.generated.h"

namespace EEndPlayReason { enum Type : int; }

class AActor;
class UObject;

/**
 * AMiraiHUD
 *
 *  Note that you typically do not need to extend or modify this class, instead you would
 *  use an "Add Widget" action in your experience to add a HUD layout and widgets to it
 * 
 *  This class exists primarily for debug rendering
 */
UCLASS(Config = Game)
class AMiraiHUD : public AHUD
{
	GENERATED_BODY()

// AMiraiHUD
public:
	AMiraiHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

// UObject
protected:
	virtual void PreInitializeComponents() override;

// AActor
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
// AHUD
protected:
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
};
