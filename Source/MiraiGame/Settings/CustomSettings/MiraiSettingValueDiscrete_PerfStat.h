// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameSettingValueDiscrete.h"

#include "MiraiSettingValueDiscrete_PerfStat.generated.h"

enum class EMiraiDisplayablePerformanceStat : uint8;
enum class EMiraiStatDisplayMode : uint8;

class UObject;

UCLASS()
class UMiraiSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UMiraiSettingValueDiscrete_PerfStat();

	void SetStat(EMiraiDisplayablePerformanceStat InStat);

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	
	void AddMode(FText&& Label, EMiraiStatDisplayMode Mode);
protected:
	TArray<FText> Options;
	TArray<EMiraiStatDisplayMode> DisplayModes;

	EMiraiDisplayablePerformanceStat StatToDisplay;
	EMiraiStatDisplayMode InitialMode;
};
