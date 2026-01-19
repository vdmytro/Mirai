// Copyright (c) 2025, dvolkov. All rights reserved.

#include "Loot/MiraiLoot.h"


AMiraiLoot::AMiraiLoot()
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}


void AMiraiLoot::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMiraiLoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

