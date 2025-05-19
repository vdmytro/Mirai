// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiContextEffectsLibraryFactory.h"

#include "Feedback/ContextEffects/MiraiContextEffectsLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiContextEffectsLibraryFactory)

class FFeedbackContext;
class UClass;
class UObject;

UMiraiContextEffectsLibraryFactory::UMiraiContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UMiraiContextEffectsLibrary::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UMiraiContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMiraiContextEffectsLibrary* MiraiContextEffectsLibrary = NewObject<UMiraiContextEffectsLibrary>(InParent, Name, Flags);

	return MiraiContextEffectsLibrary;
}
