// Copyright(c) 2025, dvolkov.All rights reserved.

#include "UI/Foundation/MiraiLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// UMiraiLoadingScreenSubsystem

UMiraiLoadingScreenSubsystem::UMiraiLoadingScreenSubsystem()
{
}

void UMiraiLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> UMiraiLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}

