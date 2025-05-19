// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"

class UClass;

class FAssetTypeActions_MiraiContextEffectsLibrary : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_MiraiContextEffectsLibrary", "MiraiContextEffectsLibrary"); }
	virtual FColor GetTypeColor() const override { return FColor(65, 200, 98); }
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Gameplay; }
};
