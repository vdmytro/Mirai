// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"

#include "MiraiIndicatorManagerComponent.generated.h"

class AController;
class UIndicatorDescriptor;
class UObject;
struct FFrame;

/**
 * @class UMiraiIndicatorManagerComponent
 */
UCLASS(BlueprintType, Blueprintable)
class MIRAIGAME_API UMiraiIndicatorManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UMiraiIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UMiraiIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(UMiraiIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
};
