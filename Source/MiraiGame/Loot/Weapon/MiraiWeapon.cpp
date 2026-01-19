// Copyright (c) 2025, dvolkov. All rights reserved.


#include "Loot/Weapon/MiraiWeapon.h"
#include "Loot/Weapon/MiraiWeaponAttachment.h"



AMiraiWeapon::AMiraiWeapon()
{
	BodySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodySkeletalMesh"));
	BodySkeletalMesh->SetupAttachment(RootComponent);

	RearSightSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RearSightSkeletalMesh"));
	RearSightSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("RearSightSocket"));

	HandguardSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandguardSkeletalMesh"));
	HandguardSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("HandguardPivotSocket"));

	MuzzleSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MuzzleSkeletalMesh"));
	MuzzleSkeletalMesh->SetupAttachment(HandguardSkeletalMesh, TEXT("MuzzlePivotSocket"));

	AttachmentDownSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachmentDownSkeletalMesh"));
	AttachmentDownSkeletalMesh->SetupAttachment(HandguardSkeletalMesh, TEXT("AttachmentDownPivotSocket"));

	AttachmentUpSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachmentUpSkeletalMesh"));
	AttachmentUpSkeletalMesh->SetupAttachment(HandguardSkeletalMesh, TEXT("AttachmentUpPivotSocket"));

	AttachmentLeftSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachmentLeftSkeletalMesh"));
	AttachmentLeftSkeletalMesh->SetupAttachment(HandguardSkeletalMesh, TEXT("AttachmentLeftPivotSocket"));

	AttachmentRightSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachmentRightSkeletalMesh"));
	AttachmentRightSkeletalMesh->SetupAttachment(HandguardSkeletalMesh, TEXT("AttachmentRightPivotSocket"));

	FrontSightSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FrontSightSkeletalMesh"));
	FrontSightSkeletalMesh->SetupAttachment(HandguardSkeletalMesh, TEXT("FrontSightPivotSocket"));
	
	StockSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StockSkeletalMesh"));
	StockSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("StockPivotSocket"));

	MagazineSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagazineSkeletalMesh"));
	MagazineSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("MagazinePivotSocket"));

	GripSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GripSkeletalMesh"));
	GripSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("GripPivotSocket"));

	ScopeSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ScopeSkeletalMesh"));
	ScopeSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("ScopePivotSocket"));

	TriggerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TriggerSkeletalMesh"));
	TriggerSkeletalMesh->SetupAttachment(BodySkeletalMesh, TEXT("TriggerPivotSocket"));
}


void AMiraiWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMiraiWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

