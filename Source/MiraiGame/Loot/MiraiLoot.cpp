// Copyright (c) 2025, dvolkov. All rights reserved.

#include "Loot/MiraiLoot.h"

#include "Components/BoxComponent.h"


AMiraiLoot::AMiraiLoot()
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;
	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollisionComponent"));
	BoxComponent->SetSimulatePhysics(true);
	RootComponent = BoxComponent;
}


void AMiraiLoot::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMiraiLoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

