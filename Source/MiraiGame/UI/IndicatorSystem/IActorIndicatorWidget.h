// Copyright(c) 2025, dvolkov.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "IActorIndicatorWidget.generated.h"

class AActor;
class UIndicatorDescriptor;

UINTERFACE(BlueprintType)
class MIRAIGAME_API UIndicatorWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class IIndicatorWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Indicator")
	void BindIndicator(UIndicatorDescriptor* Indicator);

	UFUNCTION(BlueprintNativeEvent, Category = "Indicator")
	void UnbindIndicator(const UIndicatorDescriptor* Indicator);
};
