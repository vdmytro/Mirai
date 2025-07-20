// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiWidgetFactory.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiWidgetFactory)

class UUserWidget;

TSubclassOf<UUserWidget> UMiraiWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}
