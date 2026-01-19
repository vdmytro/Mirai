// Copyright(c) 2025, dvolkov.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AttributeSet.h"

#include "AttributeSet_Hero.generated.h"

UCLASS(Blueprintable, BlueprintType, meta=(HideInDetailsView))
class MIRAIGAME_API UAttributeSet_Hero : public UAttributeSet
{
	GENERATED_UCLASS_BODY()
 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Attribute", meta = (HideFromModifiers))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Attribute", meta = (HideFromModifiers))
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Attribute", meta = (HideFromModifiers))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Attribute", meta = (HideFromModifiers))
	FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Attribute")
	FGameplayAttributeData Strength;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};


