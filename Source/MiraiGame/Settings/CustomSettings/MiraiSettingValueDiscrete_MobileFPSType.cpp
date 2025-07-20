// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiSettingValueDiscrete_MobileFPSType.h"

#include "Performance/MiraiPerformanceSettings.h"
#include "Settings/MiraiSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "MiraiSettings"

UMiraiSettingValueDiscrete_MobileFPSType::UMiraiSettingValueDiscrete_MobileFPSType()
{
}

void UMiraiSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const UMiraiPlatformSpecificRenderingSettings* PlatformSettings = UMiraiPlatformSpecificRenderingSettings::Get();
	const UMiraiSettingsLocal* UserSettings = UMiraiSettingsLocal::Get();

	for (int32 TestLimit : PlatformSettings->MobileFrameRateLimits)
	{
		if (UMiraiSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 UMiraiSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return UMiraiSettingsLocal::GetDefaultMobileFrameRate();
}

FText UMiraiSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

void UMiraiSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void UMiraiSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void UMiraiSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void UMiraiSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 UMiraiSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> UMiraiSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

int32 UMiraiSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return UMiraiSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void UMiraiSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	UMiraiSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

#undef LOCTEXT_NAMESPACE

