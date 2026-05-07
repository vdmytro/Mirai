// Copyright Epic Games, Inc. All Rights Reserved.

#include "Loot/MiraiLoot.h"



AMiraiLoot::AMiraiLoot()
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;
}


void AMiraiLoot::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMiraiLoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

