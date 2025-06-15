// Fill out your copyright notice in the Description page of Project Settings.

#include "MiraiAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "Character/MiraiCharacter.h"
#include "Character/MiraiCharacterMovementComponent.h"
#include "MiraiLogChannels.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiAnimInstance)


UMiraiAnimInstance::UMiraiAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMiraiAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UMiraiAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UMiraiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
		if (ASC)
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UMiraiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AMiraiCharacter* Character = Cast<AMiraiCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UMiraiCharacterMovementComponent* CharMoveComp = Cast<UMiraiCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMoveComp)
	{
		const FMiraiCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
		GroundDistance = GroundInfo.GroundDistance;
	}
	else
	{
		UE_LOG(LogMiraiAnimation, Error, TEXT("Can't cast MovementComp to UMiraiCharacterMovementComponent"));
	}
}




