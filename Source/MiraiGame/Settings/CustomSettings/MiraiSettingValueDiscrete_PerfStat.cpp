// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiSettingValueDiscrete_PerfStat.h"

#include "CommonUIVisibilitySubsystem.h"
#include "Performance/MiraiPerformanceSettings.h"
#include "Performance/MiraiPerformanceStatTypes.h"
#include "Settings/MiraiSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiSettingValueDiscrete_PerfStat)

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "MiraiSettings"

//////////////////////////////////////////////////////////////////////

class FGameSettingEditCondition_PerfStatAllowed : public FGameSettingEditCondition
{
public:
	static TSharedRef<FGameSettingEditCondition_PerfStatAllowed> Get(EMiraiDisplayablePerformanceStat Stat)
	{
		return MakeShared<FGameSettingEditCondition_PerfStatAllowed>(Stat);
	}

	FGameSettingEditCondition_PerfStatAllowed(EMiraiDisplayablePerformanceStat Stat)
		: AssociatedStat(Stat)
	{
	}

	//~FGameSettingEditCondition interface
	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGameSettingEditableState& InOutEditState) const override
	{
		const FGameplayTagContainer& VisibilityTags = UCommonUIVisibilitySubsystem::GetChecked(InLocalPlayer)->GetVisibilityTags();

		bool bCanShowStat = false;
		for (const FMiraiPerformanceStatGroup& Group : GetDefault<UMiraiPerformanceSettings>()->UserFacingPerformanceStats) //@TODO: Move this stuff to per-platform instead of doing vis queries too?
		{
			if (Group.AllowedStats.Contains(AssociatedStat))
			{
				const bool bShowGroup = (Group.VisibilityQuery.IsEmpty() || Group.VisibilityQuery.Matches(VisibilityTags));
				if (bShowGroup)
				{
					bCanShowStat = true;
					break;
				}
			}
		}

		if (!bCanShowStat)
		{
			InOutEditState.Hide(TEXT("Stat is not listed in UMiraiPerformanceSettings or is suppressed by current platform traits"));
		}
	}
	//~End of FGameSettingEditCondition interface

private:
	EMiraiDisplayablePerformanceStat AssociatedStat;
};

//////////////////////////////////////////////////////////////////////

UMiraiSettingValueDiscrete_PerfStat::UMiraiSettingValueDiscrete_PerfStat()
{
}

void UMiraiSettingValueDiscrete_PerfStat::SetStat(EMiraiDisplayablePerformanceStat InStat)
{
	StatToDisplay = InStat;
	SetDevName(FName(*FString::Printf(TEXT("PerfStat_%d"), (int32)StatToDisplay)));
	AddEditCondition(FGameSettingEditCondition_PerfStatAllowed::Get(StatToDisplay));
}

void UMiraiSettingValueDiscrete_PerfStat::AddMode(FText&& Label, EMiraiStatDisplayMode Mode)
{
	Options.Emplace(MoveTemp(Label));
	DisplayModes.Add(Mode);
}

void UMiraiSettingValueDiscrete_PerfStat::OnInitialized()
{
	Super::OnInitialized();

	AddMode(LOCTEXT("PerfStatDisplayMode_None", "None"), EMiraiStatDisplayMode::Hidden);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextOnly", "Text Only"), EMiraiStatDisplayMode::TextOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_GraphOnly", "Graph Only"), EMiraiStatDisplayMode::GraphOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextAndGraph", "Text and Graph"), EMiraiStatDisplayMode::TextAndGraph);
}

void UMiraiSettingValueDiscrete_PerfStat::StoreInitial()
{
	const UMiraiSettingsLocal* Settings = UMiraiSettingsLocal::Get();
	InitialMode = Settings->GetPerfStatDisplayState(StatToDisplay);
}

void UMiraiSettingValueDiscrete_PerfStat::ResetToDefault()
{
	UMiraiSettingsLocal* Settings = UMiraiSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, EMiraiStatDisplayMode::Hidden);
	NotifySettingChanged(EGameSettingChangeReason::ResetToDefault);
}

void UMiraiSettingValueDiscrete_PerfStat::RestoreToInitial()
{
	UMiraiSettingsLocal* Settings = UMiraiSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, InitialMode);
	NotifySettingChanged(EGameSettingChangeReason::RestoreToInitial);
}

void UMiraiSettingValueDiscrete_PerfStat::SetDiscreteOptionByIndex(int32 Index)
{
	if (DisplayModes.IsValidIndex(Index))
	{
		const EMiraiStatDisplayMode DisplayMode = DisplayModes[Index];

		UMiraiSettingsLocal* Settings = UMiraiSettingsLocal::Get();
		Settings->SetPerfStatDisplayState(StatToDisplay, DisplayMode);
	}
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 UMiraiSettingValueDiscrete_PerfStat::GetDiscreteOptionIndex() const
{
	const UMiraiSettingsLocal* Settings = UMiraiSettingsLocal::Get();
	return DisplayModes.Find(Settings->GetPerfStatDisplayState(StatToDisplay));
}

TArray<FText> UMiraiSettingValueDiscrete_PerfStat::GetDiscreteOptions() const
{
	return Options;
}

#undef LOCTEXT_NAMESPACE
