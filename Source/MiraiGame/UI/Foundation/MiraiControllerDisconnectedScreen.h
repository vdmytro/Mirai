// Copyright(c) 2025, dvolkov.All rights reserved.

#pragma once

#include "CommonActivatableWidget.h"

#include "MiraiControllerDisconnectedScreen.generated.h"

class UHorizontalBox;
class UObject;

UCLASS(Abstract, BlueprintType, Blueprintable)
class UMiraiControllerDisconnectedScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	void NativeOnActivated() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HBox_SwitchUser;
};
