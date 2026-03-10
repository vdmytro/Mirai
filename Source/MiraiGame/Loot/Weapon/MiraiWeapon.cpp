// Copyright (c) 2025, dvolkov. All rights reserved.


#include "Loot/Weapon/MiraiWeapon.h"


#include "Loot/Weapon/MiraiWeaponAttachment.h"
#include "MiraiLogChannels.h"
#include "System/LootData/Definitions/MiraiWeaponDefinition.h"
#include "System/LootData/Definitions/MiraiAttachmentDefiniotion.h"



AMiraiWeapon::AMiraiWeapon()
{
}


void AMiraiWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMiraiWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	

    RebuildMeshes();
}

void AMiraiWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void AMiraiWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMiraiWeapon::RebuildMeshes()
{
    for (FAttachmentSlot& AttachmentSlot : AttachmentDataList)
    {
        if(IsValid(AttachmentSlot.SkeletalMeshComponent))
        { 
            AttachmentSlot.SkeletalMeshComponent->DestroyComponent();
        }
    }

    for (FAttachmentSlot& AttachmentSlot : AttachmentDataList)
    {
        UMiraiAttachmentDefiniotion* Data = AttachmentSlot.DataAsset.LoadSynchronous();

        USkeletalMeshComponent* NewComponent = NewObject<USkeletalMeshComponent>(this);

        NewComponent->RegisterComponent();
        NewComponent->SetSkeletalMesh(Data->SkeletalMesh);
        NewComponent->AttachToComponent( 
            AttachmentSlot.Parentindex >=0 ? AttachmentDataList[AttachmentSlot.Parentindex].SkeletalMeshComponent : RootComponent,
            FAttachmentTransformRules::SnapToTargetIncludingScale,
            AttachmentSlot.SocketName);

        AttachmentSlot.SkeletalMeshComponent = NewComponent;
    }
}