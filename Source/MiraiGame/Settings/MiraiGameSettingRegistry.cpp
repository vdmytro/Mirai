// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "MiraiSettingsLocal.h"
#include "MiraiSettingsShared.h"
#include "Player/MiraiLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogMiraiGameSettingRegistry);

#define LOCTEXT_NAMESPACE "Mirai"

//--------------------------------------
// UMiraiGameSettingRegistry
//--------------------------------------

UMiraiGameSettingRegistry::UMiraiGameSettingRegistry()
{
}

UMiraiGameSettingRegistry* UMiraiGameSettingRegistry::Get(UMiraiLocalPlayer* InLocalPlayer)
{
	UMiraiGameSettingRegistry* Registry = FindObject<UMiraiGameSettingRegistry>(InLocalPlayer, TEXT("MiraiGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UMiraiGameSettingRegistry>(InLocalPlayer, TEXT("MiraiGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool UMiraiGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (UMiraiLocalPlayer* LocalPlayer = Cast<UMiraiLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

void UMiraiGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UMiraiLocalPlayer* MiraiLocalPlayer = Cast<UMiraiLocalPlayer>(InLocalPlayer);

	VideoSettings = InitializeVideoSettings(MiraiLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, MiraiLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(MiraiLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(MiraiLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(MiraiLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(MiraiLocalPlayer);
	RegisterSetting(GamepadSettings);
}

void UMiraiGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (UMiraiLocalPlayer* LocalPlayer = Cast<UMiraiLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

#undef LOCTEXT_NAMESPACE

