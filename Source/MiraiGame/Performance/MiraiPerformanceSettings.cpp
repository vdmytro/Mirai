// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "Performance/MiraiPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPerformanceSettings)

//////////////////////////////////////////////////////////////////////

UMiraiPlatformSpecificRenderingSettings::UMiraiPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UMiraiPlatformSpecificRenderingSettings* UMiraiPlatformSpecificRenderingSettings::Get()
{
	UMiraiPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UMiraiPerformanceSettings::UMiraiPerformanceSettings()
{
	PerPlatformSettings.Initialize(UMiraiPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FMiraiPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EMiraiDisplayablePerformanceStat PerfStat : TEnumRange<EMiraiDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

