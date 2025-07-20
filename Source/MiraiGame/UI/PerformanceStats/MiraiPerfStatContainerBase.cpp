// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPerfStatContainerBase.h"

#include "Blueprint/WidgetTree.h"
#include "MiraiPerfStatWidgetBase.h"
#include "Settings/MiraiSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPerfStatContainerBase)

//////////////////////////////////////////////////////////////////////
// UMiraiPerfStatsContainerBase

UMiraiPerfStatContainerBase::UMiraiPerfStatContainerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMiraiPerfStatContainerBase::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateVisibilityOfChildren();

	UMiraiSettingsLocal::Get()->OnPerfStatDisplayStateChanged().AddUObject(this, &ThisClass::UpdateVisibilityOfChildren);
}

void UMiraiPerfStatContainerBase::NativeDestruct()
{
	UMiraiSettingsLocal::Get()->OnPerfStatDisplayStateChanged().RemoveAll(this);

	Super::NativeDestruct();
}

void UMiraiPerfStatContainerBase::UpdateVisibilityOfChildren()
{
	UMiraiSettingsLocal* UserSettings = UMiraiSettingsLocal::Get();

	const bool bShowTextWidgets = (StatDisplayModeFilter == EMiraiStatDisplayMode::TextOnly) || (StatDisplayModeFilter == EMiraiStatDisplayMode::TextAndGraph);
	const bool bShowGraphWidgets = (StatDisplayModeFilter == EMiraiStatDisplayMode::GraphOnly) || (StatDisplayModeFilter == EMiraiStatDisplayMode::TextAndGraph);
	
	check(WidgetTree);
	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UMiraiPerfStatWidgetBase* TypedWidget = Cast<UMiraiPerfStatWidgetBase>(Widget))
		{
			const EMiraiStatDisplayMode SettingMode = UserSettings->GetPerfStatDisplayState(TypedWidget->GetStatToDisplay());

			bool bShowWidget = false;
			switch (SettingMode)
			{
			case EMiraiStatDisplayMode::Hidden:
				bShowWidget = false;
				break;
			case EMiraiStatDisplayMode::TextOnly:
				bShowWidget = bShowTextWidgets;
				break;
			case EMiraiStatDisplayMode::GraphOnly:
				bShowWidget = bShowGraphWidgets;
				break;
			case EMiraiStatDisplayMode::TextAndGraph:
				bShowWidget = bShowTextWidgets || bShowGraphWidgets;
				break;
			}

			TypedWidget->SetVisibility(bShowWidget ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	});
}

