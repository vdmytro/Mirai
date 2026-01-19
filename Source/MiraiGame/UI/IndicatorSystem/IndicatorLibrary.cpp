// Copyright(c) 2025, dvolkov.All rights reserved.

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

