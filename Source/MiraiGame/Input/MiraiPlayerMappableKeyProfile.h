// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UserSettings/EnhancedInputUserSettings.h"

#include "MiraiPlayerMappableKeyProfile.generated.h"

UCLASS()
class MIRAIGAME_API UMiraiPlayerMappableKeyProfile : public UEnhancedPlayerMappableKeyProfile
{
	GENERATED_BODY()

protected:

	//~ Begin UEnhancedPlayerMappableKeyProfile interface
	virtual void EquipProfile() override;
	virtual void UnEquipProfile() override;
	//~ End UEnhancedPlayerMappableKeyProfile interface
};