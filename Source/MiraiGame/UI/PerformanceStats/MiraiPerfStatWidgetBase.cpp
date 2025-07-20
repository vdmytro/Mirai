// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPerfStatWidgetBase.h"

#include "Engine/GameInstance.h"
#include "Performance/MiraiPerformanceStatSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPerfStatWidgetBase)

//////////////////////////////////////////////////////////////////////
// UMiraiPerfStatWidgetBase

UMiraiPerfStatWidgetBase::UMiraiPerfStatWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

double UMiraiPerfStatWidgetBase::FetchStatValue()
{
	if (CachedStatSubsystem == nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				CachedStatSubsystem = GameInstance->GetSubsystem<UMiraiPerformanceStatSubsystem>();
			}
		}
	}

	if (CachedStatSubsystem)
	{
		return CachedStatSubsystem->GetCachedStat(StatToDisplay);
	}
	else
	{
		return 0.0;
	}
}

