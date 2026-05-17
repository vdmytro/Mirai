// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "EditCondition/WhenPlatformHasTrait.h"
#include "CustomSettings/MiraiSettingValueDiscrete_Language.h"
#include "MiraiSettingsLocal.h"
#include "GameSettingValueDiscreteDynamic.h"
#include "Player/MiraiLocalPlayer.h"

#define LOCTEXT_NAMESPACE "Mirai"

UGameSettingCollection* UMiraiGameSettingRegistry::InitializeGameplaySettings(UMiraiLocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("GameplayCollection"));
	Screen->SetDisplayName(LOCTEXT("GameplayCollection_Name", "Gameplay"));
	Screen->Initialize(InLocalPlayer);

	{
		UGameSettingCollection* LanguageSubsection = NewObject<UGameSettingCollection>();
		LanguageSubsection->SetDevName(TEXT("LanguageCollection"));
		LanguageSubsection->SetDisplayName(LOCTEXT("LanguageCollection_Name", "Language"));
		Screen->AddSetting(LanguageSubsection);

		//----------------------------------------------------------------------------------
		{
			UMiraiSettingValueDiscrete_Language* Setting = NewObject<UMiraiSettingValueDiscrete_Language>();
			Setting->SetDevName(TEXT("Language"));
			Setting->SetDisplayName(LOCTEXT("LanguageSetting_Name", "Language"));
			Setting->SetDescriptionRichText(LOCTEXT("LanguageSetting_Description", "The language of the game."));
			
#if WITH_EDITOR
			if (GIsEditor)
			{
				Setting->SetDescriptionRichText(LOCTEXT("LanguageSetting_WithEditor_Description", "The language of the game.\n\n<text color=\"#ffff00\">WARNING: Language changes will not affect PIE, you'll need to run with -game to test this, or change your PIE language options in the editor preferences.</>"));
			}
#endif
			
			Setting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			LanguageSubsection->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
	}

	return Screen;
}

#undef LOCTEXT_NAMESPACE
