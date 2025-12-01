// Copyright Epic Games, Inc. All Rights Reserved.

#include "IndicatorLibrary.h"

#include "MiraiIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorLibrary)

class AController;

UIndicatorLibrary::UIndicatorLibrary()
{
}

UMiraiIndicatorManagerComponent* UIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return UMiraiIndicatorManagerComponent::GetComponent(Controller);
}

