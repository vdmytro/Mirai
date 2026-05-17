// Copyright Epic Games, Inc. All Rights Reserved.


#include "Loot/Weapon/MiraiWeapon.h"


#include "MiraiLogChannels.h"
#include "System/LootData/Definitions/MiraiWeaponDefinition.h"
#include "System/LootData/Definitions/MiraiAttachmentDefinition.h"
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
    RootComponent->DestroyComponent(true);

    FTransform RootTransform = RootComponent->GetComponentTransform();
    USkeletalMeshComponent* NewRootComponent = NewObject<USkeletalMeshComponent>(this);
    NewRootComponent->RegisterComponent();

    UMiraiWeaponDefinition* WeaponDataAsset = DataAsset.LoadSynchronous();
    NewRootComponent->SetSkeletalMesh(WeaponDataAsset->BaseSkeletalMesh);

    RootComponent = NewRootComponent;
    FHitResult HitResult;

    if (HasAuthority())
    {
        NewRootComponent->SetCollisionProfileName(TEXT("Weapon"));
        NewRootComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        NewRootComponent->SetSimulatePhysics(true);
        RootComponent->K2_SetWorldTransform(RootTransform, false, HitResult, true);
    }

    for (FAttachmentSlot& AttachmentSlot : DataArray.AttachmentDataList)
    {

        UMiraiAttachmentDefinition* Data = AttachmentSlot.DataAsset.LoadSynchronous();
        UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(this);
        NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachmentSlot.SocketName);

        NewComponent->SetStaticMesh(Data->Mesh);
    }
}

void AMiraiWeapon::OnRep_DataArray()
{

}