// Copyright CtrlCtrlV, Inc. All Rights Reserved.

#include "MiraiCharacter.h"


#include "AbilitySystem/MiraiAbilitySystemComponent.h"
#include "Camera/MiraiCameraComponent.h"
#include "Character/MiraiPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MiraiCharacterMovementComponent.h"
#include "MiraiGameplayTags.h"
#include "MiraiLogChannels.h"
#include "Net/UnrealNetwork.h"
#include "Player/MiraiPlayerController.h"
#include "Player/MiraiPlayerState.h"
#include "System/MiraiSignificanceManager.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiCharacter)

class AActor;
class FLifetimeProperty;
class IRepChangedPropertyTracker;
class UInputComponent;

static FName NAME_MiraiCharacterCollisionProfile_Capsule(TEXT("MiraiPawnCapsule"));
static FName NAME_MiraiCharacterCollisionProfile_Mesh(TEXT("MiraiPawnMesh"));


AMiraiCharacter::AMiraiCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMiraiCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_MiraiCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_MiraiCharacterCollisionProfile_Mesh);

	UMiraiCharacterMovementComponent* MiraiMoveComp = CastChecked<UMiraiCharacterMovementComponent>(GetCharacterMovement());
	MiraiMoveComp->GravityScale = 1.0f;
	MiraiMoveComp->MaxAcceleration = 2400.0f;
	MiraiMoveComp->BrakingFrictionFactor = 1.0f;
	MiraiMoveComp->BrakingFriction = 6.0f;
	MiraiMoveComp->GroundFriction = 8.0f;
	MiraiMoveComp->BrakingDecelerationWalking = 1400.0f;
	MiraiMoveComp->bUseControllerDesiredRotation = false;
	MiraiMoveComp->bOrientRotationToMovement = false;
	MiraiMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MiraiMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MiraiMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	MiraiMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MiraiMoveComp->SetCrouchedHalfHeight(65.0f);

	PawnExtComponent = CreateDefaultSubobject<UMiraiPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	CameraComponent = CreateDefaultSubobject<UMiraiCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

void AMiraiCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMiraiCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMiraiCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void AMiraiCharacter::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UninitAndDestroy();
}

void AMiraiCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
}

void AMiraiCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void AMiraiCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

AMiraiPlayerController* AMiraiCharacter::GetMiraiPlayerController() const
{
	return CastChecked<AMiraiPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AMiraiPlayerState* AMiraiCharacter::GetMiraiPlayerState() const
{
	return CastChecked<AMiraiPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UMiraiAbilitySystemComponent* AMiraiCharacter::GetMiraiAbilitySystemComponent() const
{
	return Cast<UMiraiAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AMiraiCharacter::GetAbilitySystemComponent() const
{
	if (!PawnExtComponent)
	{
		UE_LOG(LogMiraiCharacter, Error, TEXT("PawnExtComponent not initialized."));
		return nullptr;
	}

	return PawnExtComponent->GetMiraiAbilitySystemComponent();
}

void AMiraiCharacter::OnAbilitySystemInitialized()
{
	InitializeGameplayTags();
}

void AMiraiCharacter::OnAbilitySystemUninitialized()
{
}

void AMiraiCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void AMiraiCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void AMiraiCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void AMiraiCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}

void AMiraiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void AMiraiCharacter::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		for (const TPair<uint8, FGameplayTag>& TagMapping : MiraiGameplayTags::MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				MiraiASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : MiraiGameplayTags::CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				MiraiASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UMiraiCharacterMovementComponent* MiraiMoveComp = CastChecked<UMiraiCharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(MiraiMoveComp->MovementMode, MiraiMoveComp->CustomMovementMode, true);
	}
}

void AMiraiCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		MiraiASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool AMiraiCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		return MiraiASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool AMiraiCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		return MiraiASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool AMiraiCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		return MiraiASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void AMiraiCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
}

void AMiraiCharacter::OnDeathStarted(AActor* Actor)
{
	DisableMovementAndCollision();
}

void AMiraiCharacter::OnDeathFinished(AActor* Actor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void AMiraiCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UMiraiCharacterMovementComponent* MiraiMoveComp = CastChecked<UMiraiCharacterMovementComponent>(GetCharacterMovement());
	MiraiMoveComp->StopMovementImmediately();
	MiraiMoveComp->DisableMovement();
}

void AMiraiCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}


void AMiraiCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		if (MiraiASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void AMiraiCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UMiraiCharacterMovementComponent* MiraiMoveComp = CastChecked<UMiraiCharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(MiraiMoveComp->MovementMode, MiraiMoveComp->CustomMovementMode, true);
}

void AMiraiCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = MiraiGameplayTags::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = MiraiGameplayTags::MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			MiraiASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void AMiraiCharacter::ToggleCrouch()
{
	const UMiraiCharacterMovementComponent* MiraiMoveComp = CastChecked<UMiraiCharacterMovementComponent>(GetCharacterMovement());

	if (bIsCrouched || MiraiMoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (MiraiMoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void AMiraiCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		MiraiASC->SetLooseGameplayTagCount(MiraiGameplayTags::Status_Crouching, 1);
	}


	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AMiraiCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UMiraiAbilitySystemComponent* MiraiASC = GetMiraiAbilitySystemComponent())
	{
		MiraiASC->SetLooseGameplayTagCount(MiraiGameplayTags::Status_Crouching, 0);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool AMiraiCharacter::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return JumpIsAllowedInternal();
}

void AMiraiCharacter::OnRep_ReplicatedAcceleration()
{
	if (UMiraiCharacterMovementComponent* MiraiMovementComponent = Cast<UMiraiCharacterMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel = MiraiMovementComponent->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		MiraiMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}

bool AMiraiCharacter::UpdateSharedReplication()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FSharedRepMovement SharedMovement;
		if (SharedMovement.FillForCharacter(this))
		{
			// Only call FastSharedReplication if data has changed since the last frame.
			// Skipping this call will cause replication to reuse the same bunch that we previously
			// produced, but not send it to clients that already received. (But a new client who has not received
			// it, will get it this frame)
			if (!SharedMovement.Equals(LastSharedReplication, this))
			{
				LastSharedReplication = SharedMovement;
				ReplicatedMovementMode = SharedMovement.RepMovementMode;

				FastSharedReplication(SharedMovement);
			}
			return true;
		}
	}

	// We cannot fastrep right now. Don't send anything.
	return false;
}

void AMiraiCharacter::FastSharedReplication_Implementation(const FSharedRepMovement& SharedRepMovement)
{
	if (GetWorld()->IsPlayingReplay())
	{
		return;
	}

	// Timestamp is checked to reject old moves.
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Timestamp
		ReplicatedServerLastTransformUpdateTimeStamp = SharedRepMovement.RepTimeStamp;

		// Movement mode
		if (ReplicatedMovementMode != SharedRepMovement.RepMovementMode)
		{
			ReplicatedMovementMode = SharedRepMovement.RepMovementMode;
			GetCharacterMovement()->bNetworkMovementModeChanged = true;
			GetCharacterMovement()->bNetworkUpdateReceived = true;
		}

		// Location, Rotation, Velocity, etc.
		FRepMovement& MutableRepMovement = GetReplicatedMovement_Mutable();
		MutableRepMovement = SharedRepMovement.RepMovement;

		// This also sets LastRepMovement
		OnRep_ReplicatedMovement();

		// Jump force
		bProxyIsJumpForceApplied = SharedRepMovement.bProxyIsJumpForceApplied;

		// Crouch
		if (bIsCrouched != SharedRepMovement.bIsCrouched)
		{
			bIsCrouched = SharedRepMovement.bIsCrouched;
			OnRep_IsCrouched();
		}
	}
}

FSharedRepMovement::FSharedRepMovement()
{
	RepMovement.LocationQuantizationLevel = EVectorQuantization::RoundTwoDecimals;
}

bool FSharedRepMovement::FillForCharacter(ACharacter* Character)
{
	if (USceneComponent* PawnRootComponent = Character->GetRootComponent())
	{
		UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();

		RepMovement.Location = FRepMovement::RebaseOntoZeroOrigin(PawnRootComponent->GetComponentLocation(), Character);
		RepMovement.Rotation = PawnRootComponent->GetComponentRotation();
		RepMovement.LinearVelocity = CharacterMovement->Velocity;
		RepMovementMode = CharacterMovement->PackNetworkMovementMode();
		bProxyIsJumpForceApplied = Character->bProxyIsJumpForceApplied || (Character->JumpForceTimeRemaining > 0.0f);
		bIsCrouched = Character->bIsCrouched;

		// Timestamp is sent as zero if unused
		if ((CharacterMovement->NetworkSmoothingMode == ENetworkSmoothingMode::Linear) || CharacterMovement->bNetworkAlwaysReplicateTransformUpdateTimestamp)
		{
			RepTimeStamp = CharacterMovement->GetServerLastTransformUpdateTimeStamp();
		}
		else
		{
			RepTimeStamp = 0.f;
		}

		return true;
	}
	return false;
}

bool FSharedRepMovement::Equals(const FSharedRepMovement& Other, ACharacter* Character) const
{
	if (RepMovement.Location != Other.RepMovement.Location)
	{
		return false;
	}

	if (RepMovement.Rotation != Other.RepMovement.Rotation)
	{
		return false;
	}

	if (RepMovement.LinearVelocity != Other.RepMovement.LinearVelocity)
	{
		return false;
	}

	if (RepMovementMode != Other.RepMovementMode)
	{
		return false;
	}

	if (bProxyIsJumpForceApplied != Other.bProxyIsJumpForceApplied)
	{
		return false;
	}

	if (bIsCrouched != Other.bIsCrouched)
	{
		return false;
	}

	return true;
}

bool FSharedRepMovement::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	RepMovement.NetSerialize(Ar, Map, bOutSuccess);
	Ar << RepMovementMode;
	Ar << bProxyIsJumpForceApplied;
	Ar << bIsCrouched;

	// Timestamp, if non-zero.
	uint8 bHasTimeStamp = (RepTimeStamp != 0.f);
	Ar.SerializeBits(&bHasTimeStamp, 1);
	if (bHasTimeStamp)
	{
		Ar << RepTimeStamp;
	}
	else
	{
		RepTimeStamp = 0.f;
	}

	return true;
}
