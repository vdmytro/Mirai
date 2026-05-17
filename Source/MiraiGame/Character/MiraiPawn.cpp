// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPawn.h"


#include "GameFramework/Controller.h"
#include "MiraiLogChannels.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ScriptInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPawn)

class FLifetimeProperty;
class UObject;


AMiraiPawn::AMiraiPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void AMiraiPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void AMiraiPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


void AMiraiPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void AMiraiPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}


void AMiraiPawn::UnPossessed()
{
	Super::UnPossessed();
}
