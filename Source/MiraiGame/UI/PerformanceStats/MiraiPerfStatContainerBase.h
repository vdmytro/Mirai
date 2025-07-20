// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "Performance/MiraiPerformanceStatTypes.h"

#include "MiraiPerfStatContainerBase.generated.h"

class UObject;
struct FFrame;

/**
 * UMiraiPerfStatsContainerBase
 *
 * Panel that contains a set of UMiraiPerfStatWidgetBase widgets and manages
 * their visibility based on user settings.
 */
 UCLASS(Abstract)
class UMiraiPerfStatContainerBase : public UCommonUserWidget
{
public:
	UMiraiPerfStatContainerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	GENERATED_BODY()

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintCallable)
	void UpdateVisibilityOfChildren();

protected:
	// Are we showing text or graph stats?
	UPROPERTY(EditAnywhere, Category=Display)
	EMiraiStatDisplayMode StatDisplayModeFilter = EMiraiStatDisplayMode::TextAndGraph;
};
