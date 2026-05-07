// Copyright Epic Games, Inc. All Rights Reserved.


#include "Loot/Weapon/MiraiWeapon.h"


#include "Loot/Weapon/MiraiWeaponAttachment.h"
#include "MiraiLogChannels.h"
#include "System/LootData/Definitions/MiraiWeaponDefinition.h"
#include "System/LootData/Definitions/MiraiAttachmentDefiniotion.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Net/UnrealNetwork.h"



AMiraiWeapon::AMiraiWeapon()
{
    bReplicates = true;
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

bool AMiraiWeapon::Server_RebuildMeshes_Validate()
{
    // TODO: Validate asset list
    return true;
}

void AMiraiWeapon::Server_RebuildMeshes_Implementation()
{
    RebuildMeshes();
}

void AMiraiWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AMiraiWeapon, DataArray);
}

void AMiraiWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void AMiraiWeapon::RebuildMeshes()
{
    FTransform RootTransform = RootComponent->GetComponentTransform();

    UPhysicsAsset* RootPhysicsAsset;

    for (FAttachmentSlot& AttachmentSlot : DataArray.AttachmentDataList)
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
            NewComponent->AttachToComponent(DataArray.AttachmentDataList[AttachmentSlot.Parentindex].SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachmentSlot.SocketName);
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

void AMiraiWeapon::OnRep_DataArray()
{

}