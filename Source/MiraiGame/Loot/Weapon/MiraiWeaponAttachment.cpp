// Copyright Epic Games, Inc. All Rights Reserved.


#include "Loot/Weapon/MiraiWeaponAttachment.h"
#include "Loot/LootData/Definitions/MiraiAttachmentDefiniotion.h"


AMiraiWeaponAttachment::AMiraiWeaponAttachment()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	if (DataAsset && DataAsset->SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMeshAsset(DataAsset->SkeletalMesh);
	}
}


void AMiraiWeaponAttachment::BeginPlay()
{
	Super::BeginPlay();

	if (!SkeletalMeshComponent->GetSkeletalMeshAsset())
	{
		if (DataAsset && DataAsset->SkeletalMesh)
		{
			SkeletalMeshComponent->SetSkeletalMeshAsset(DataAsset->SkeletalMesh);
		}

	}
}


void AMiraiWeaponAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText AMiraiWeaponAttachment::GetName()
{
	if (DataAsset)
	{
		return DataAsset->DisplayName;
	}

	return FText::FromString(TEXT(""));
}

