// Copyright(c) 2025, dvolkov.All rights reserved.

#include "MiraiAbilitySystemGlobals.h"

#include "MiraiGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiAbilitySystemGlobals)

struct FGameplayEffectContext;

UMiraiAbilitySystemGlobals::UMiraiAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UMiraiAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMiraiGameplayEffectContext();
}

