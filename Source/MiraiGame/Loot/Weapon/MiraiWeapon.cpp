// Copyright (c) 2025, dvolkov. All rights reserved.


#include "Loot/Weapon/MiraiWeapon.h"


#include "Loot/Weapon/MiraiWeaponAttachment.h"
#include "MiraiLogChannels.h"
#include "System/LootData/Definitions/MiraiWeaponDefinition.h"
#include "System/LootData/Definitions/MiraiAttachmentDefiniotion.h"
#include "Components/BoxComponent.h"



AMiraiWeapon::AMiraiWeapon()
{
    //static_cast<UBoxComponent*>(RootComponent)->SetCollisionProfileName(TEXT("Weapon"));
    UBoxComponent* RootCollisionComponent = static_cast<UBoxComponent*>(RootComponent);
    RootCollisionComponent->SetCollisionProfileName(TEXT("Weapon"));
    RootCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
        NewComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        NewComponent->SetCollisionProfileName(TEXT("WeponAttachment"));
        if (AttachmentSlot.Parentindex >= 0)
        { 
            NewComponent->AttachToComponent(AttachmentDataList[AttachmentSlot.Parentindex].SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachmentSlot.SocketName);
        }
        else
        {
            NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachmentSlot.SocketName);;
           
        }
       
        AttachmentSlot.SkeletalMeshComponent = NewComponent;
    }

    FBox LocalBox(ForceInit);

    for (FAttachmentSlot& AttachmentSlot : AttachmentDataList)
    {

        FBox LocalMeshBox = AttachmentSlot.SkeletalMeshComponent->CalcBounds(FTransform::Identity).GetBox();

        FTransform ToRoot = AttachmentSlot.SkeletalMeshComponent->GetComponentTransform().GetRelativeTransform(RootTransform);

        const FVector Min = LocalMeshBox.Min;
        const FVector Max = LocalMeshBox.Max;

        const FVector Corners[8] =

        {
            FVector(Min.X, Min.Y, Min.Z),
            FVector(Max.X, Min.Y, Min.Z),
            FVector(Min.X, Max.Y, Min.Z),
            FVector(Max.X, Max.Y, Min.Z),
            FVector(Min.X, Min.Y, Max.Z),
            FVector(Max.X, Min.Y, Max.Z),
            FVector(Min.X, Max.Y, Max.Z),
            FVector(Max.X, Max.Y, Max.Z)
        };

        for (int i = 0; i < 8; i++)
        {
            LocalBox += ToRoot.TransformPosition(Corners[i]);
        }
    }

    static_cast<UBoxComponent*>(RootComponent)->SetBoxExtent(LocalBox.GetExtent());
}