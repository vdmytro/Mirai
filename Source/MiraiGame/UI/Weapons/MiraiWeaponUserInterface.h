// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "MiraiWeaponUserInterface.generated.h"


class UObject;
struct FGeometry;

/*
*
*
*	TODO: handle by MVVM
*
*/
UCLASS()
class UMiraiWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMiraiWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


};
