// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "MiraiPerfStatWidgetBase.generated.h"

enum class EMiraiDisplayablePerformanceStat : uint8;

class UMiraiPerformanceStatSubsystem;
class UObject;
struct FFrame;

/**
 * UMiraiPerfStatWidgetBase
 *
 * Base class for a widget that displays a single stat, e.g., FPS, ping, etc...
 */
 UCLASS(Abstract)
class UMiraiPerfStatWidgetBase : public UCommonUserWidget
{
public:
	UMiraiPerfStatWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	GENERATED_BODY()

public:
	// Returns the stat this widget is supposed to display
	UFUNCTION(BlueprintPure)
	EMiraiDisplayablePerformanceStat GetStatToDisplay() const
	{
		return StatToDisplay;
	}

	// Polls for the value of this stat (unscaled)
	UFUNCTION(BlueprintPure)
	double FetchStatValue();

protected:
	// Cached subsystem pointer
	UPROPERTY(Transient)
	TObjectPtr<UMiraiPerformanceStatSubsystem> CachedStatSubsystem;

	// The stat to display
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Display)
	EMiraiDisplayablePerformanceStat StatToDisplay;
 };
