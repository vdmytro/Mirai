// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Player/MiraiLocalPlayer.h"
#include "Settings/MiraiSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiInputComponent)

class UMiraiInputConfig;

UMiraiInputComponent::UMiraiInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UMiraiInputComponent::AddInputMappings(const UMiraiInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UMiraiInputComponent::RemoveInputMappings(const UMiraiInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UMiraiInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
