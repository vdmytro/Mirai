// Copyright (c) 2025, dvolkov. All rights reserved.


#include "Loot/Weapon/MiraiWeaponAttachment.h"


AMiraiWeaponAttachment::AMiraiWeaponAttachment()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}


void AMiraiWeaponAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMiraiWeaponAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

