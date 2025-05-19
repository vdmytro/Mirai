// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_MiraiContextEffectsLibrary.h"

#include "Feedback/ContextEffects/MiraiContextEffectsLibrary.h"

class UClass;

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_MiraiContextEffectsLibrary::GetSupportedClass() const
{
	return UMiraiContextEffectsLibrary::StaticClass();
}

#undef LOCTEXT_NAMESPACE
