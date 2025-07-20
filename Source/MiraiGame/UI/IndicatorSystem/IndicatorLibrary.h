// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "IndicatorLibrary.generated.h"

class AController;
class UMiraiIndicatorManagerComponent;
class UObject;
struct FFrame;

UCLASS()
class MIRAIGAME_API UIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UIndicatorLibrary();
	
	/**  */
	UFUNCTION(BlueprintCallable, Category = Indicator)
	static UMiraiIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller);
};
