// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "MiraiReticleWidgetBase.generated.h"

class UMiraiInventoryItemInstance;
class UMiraiWeaponInstance;
class UObject;
struct FFrame;

UCLASS(Abstract)
class UMiraiReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMiraiReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnWeaponInitialized();

	//UFUNCTION(BlueprintCallable)
	//void InitializeFromWeapon(UMiraiWeaponInstance* InWeapon);

	/** Returns the current weapon's diametrical spread angle, in degrees */
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//float ComputeSpreadAngle() const;

	/** Returns the current weapon's maximum spread radius in screenspace units (pixels) */
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//float ComputeMaxScreenspaceSpreadRadius() const;

	/**
	 * Returns true if the current weapon is at 'first shot accuracy'
	 * (the weapon allows it and it is at min spread)
	 */
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//bool HasFirstShotAccuracy() const;

protected:
	//UPROPERTY(BlueprintReadOnly)
	//TObjectPtr<UMiraiWeaponInstance> WeaponInstance;

	//UPROPERTY(BlueprintReadOnly)
	//TObjectPtr<UMiraiInventoryItemInstance> InventoryInstance;
};
