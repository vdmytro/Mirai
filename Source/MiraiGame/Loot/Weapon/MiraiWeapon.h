// Copyright (c) 2025, dvolkov. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot/MiraiLoot.h"

#include "MiraiWeapon.generated.h"


UCLASS()
class MIRAIGAME_API AMiraiWeapon : public AMiraiLoot
{
	GENERATED_BODY()
	
public:	
	AMiraiWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BodySkeletalMesh;

	// Body slots

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RearSightSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HandguardSkeletalMesh;

	// Handguard slots

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> MuzzleSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FrontSightSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> AttachmentDownSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> AttachmentUpSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> AttachmentLeftSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> AttachmentRightSkeletalMesh;

	// End Handguard slots
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> StockSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> MagazineSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GripSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ScopeSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Attachments", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> TriggerSkeletalMesh;

	// End Body slots
};
