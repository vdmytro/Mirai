// Copyright (c) 2025, dvolkov. All rights reserved.


#include "Loot/Weapon/MiraiWeapon.h"


#include "Loot/Weapon/MiraiWeaponAttachment.h"
#include "MiraiLogChannels.h"
#include "System/LootData/Definitions/MiraiWeaponDefinition.h"
#include "System/LootData/Definitions/MiraiAttachmentDefiniotion.h"
#include "PhysicsEngine/PhysicsAsset.h"



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

    if(GetWorld()->WorldType == EWorldType::EditorPreview)
    {
        RebuildMeshes();
    }

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
    FTransform RootTransform = RootComponent->GetComponentTransform();

    UPhysicsAsset* RootPhysicsAsset;

    for (FAttachmentSlot& AttachmentSlot : AttachmentDataList)
    {
        if (IsValid(AttachmentSlot.SkeletalMeshComponent))
        {
            AttachmentSlot.SkeletalMeshComponent->DestroyComponent();
        }

        UMiraiAttachmentDefiniotion* Data = AttachmentSlot.DataAsset.LoadSynchronous();

        USkeletalMeshComponent* NewComponent = NewObject<USkeletalMeshComponent>(this);

        NewComponent->RegisterComponent();
        NewComponent->SetSkeletalMesh(Data->SkeletalMesh);
        
        if (AttachmentSlot.Parentindex >= 0)
        { 
            NewComponent->AttachToComponent(AttachmentDataList[AttachmentSlot.Parentindex].SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachmentSlot.SocketName);
            // TODO: Enable or disable bodies in root PA 
        }
        else
        {
            NewComponent->SetCollisionProfileName(TEXT("Weapon"));
            NewComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            RootPhysicsAsset = NewComponent->GetPhysicsAsset();
            RootComponent = NewComponent;
            NewComponent->SetSimulatePhysics(true);
            FHitResult HitResult;
            RootComponent->K2_SetWorldTransform(RootTransform, false, HitResult, true);
        }
       
        AttachmentSlot.SkeletalMeshComponent = NewComponent;
    }
}