// Copyright Epic Games, Inc. All Rights Reserved.


#include "Loot/Weapon/MiraiWeaponAttachment.h"
#include "System/LootData/Definitions/MiraiAttachmentDefiniotion.h"


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

FString AMiraiWeaponAttachment::GetName()
{
	if (DataAsset)
	{
		return DataAsset->Name.ToString();
	}

	return TEXT("");
}

