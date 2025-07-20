// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MiraiWidgetFactory.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"

#include "MiraiWidgetFactory_Class.generated.h"

class UObject;
class UUserWidget;

UCLASS()
class MIRAIGAME_API UMiraiWidgetFactory_Class : public UMiraiWidgetFactory
{
	GENERATED_BODY()

public:
	UMiraiWidgetFactory_Class() { }

	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};
