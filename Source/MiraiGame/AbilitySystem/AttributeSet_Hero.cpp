
#include "AbilitySystem/AttributeSet_Hero.h"

#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "GameplayTagsModule.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AttributeSet_Hero)


UAttributeSet_Hero::UAttributeSet_Hero(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = MaxHealth = 100.f;
	Stamina = MaxStamina = 100.f;

	Strength = 0.f;
}

bool UAttributeSet_Hero::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return true;
}

void UAttributeSet_Hero::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	
}


void UAttributeSet_Hero::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	//DISABLE_ALL_CLASS_REPLICATED_PROPERTIES(UAbilitySystemTestAttributeSet, EFieldIteratorFlags::IncludeSuper);

	
	DOREPLIFETIME(UAttributeSet_Hero, MaxHealth);
	DOREPLIFETIME(UAttributeSet_Hero, Health);
	DOREPLIFETIME(UAttributeSet_Hero, MaxStamina);
	DOREPLIFETIME(UAttributeSet_Hero, Stamina);

	DOREPLIFETIME(UAttributeSet_Hero, Strength);
	
}

