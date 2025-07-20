// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiTabButtonBase.h"

#include "CommonLazyImage.h"
#include "UI/Common/MiraiTabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiTabButtonBase)

class UObject;
struct FSlateBrush;

void UMiraiTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UMiraiTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UMiraiTabButtonBase::SetTabLabelInfo_Implementation(const FMiraiTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}

