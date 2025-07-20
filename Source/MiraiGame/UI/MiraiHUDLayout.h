// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MiraiActivatableWidget.h"

#include "MiraiHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;


/**
 * UMiraiHUDLayout
 *
 *	Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Mirai HUD Layout", Category = "Mirai|HUD"))
class UMiraiHUDLayout : public UMiraiActivatableWidget
{
	GENERATED_BODY()

public:

	UMiraiHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
