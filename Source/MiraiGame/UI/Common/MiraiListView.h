// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonListView.h"

#include "MiraiListView.generated.h"

class UUserWidget;
class ULocalPlayer;
class UMiraiWidgetFactory;

UCLASS(meta = (DisableNativeTick))
class MIRAIGAME_API UMiraiListView : public UCommonListView
{
	GENERATED_BODY()

public:
	UMiraiListView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<UMiraiWidgetFactory>> FactoryRules;
};
