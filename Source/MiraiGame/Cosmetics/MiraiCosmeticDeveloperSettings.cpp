// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiCosmeticDeveloperSettings.h"
#include "Cosmetics/MiraiCharacterPartTypes.h"
#include "Misc/App.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "System/MiraiDevelopmentStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "MiraiControllerComponent_CharacterParts.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiCosmeticDeveloperSettings)

#define LOCTEXT_NAMESPACE "MiraiCheats"

UMiraiCosmeticDeveloperSettings::UMiraiCosmeticDeveloperSettings()
{
}

FName UMiraiCosmeticDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

#if WITH_EDITOR

void UMiraiCosmeticDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ApplySettings();
}

void UMiraiCosmeticDeveloperSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	ApplySettings();
}

void UMiraiCosmeticDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();

	ApplySettings();
}

void UMiraiCosmeticDeveloperSettings::ApplySettings()
{
	if (GIsEditor && (GEngine != nullptr))
	{
		ReapplyLoadoutIfInPIE();
	}
}

void UMiraiCosmeticDeveloperSettings::ReapplyLoadoutIfInPIE()
{
#if WITH_SERVER_CODE
	// Update the loadout on all players
	UWorld* ServerWorld = UMiraiDevelopmentStatics::FindPlayInEditorAuthorityWorld();
	if (ServerWorld != nullptr)
	{
		ServerWorld->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([=]()
			{
				for (TActorIterator<APlayerController> PCIterator(ServerWorld); PCIterator; ++PCIterator)
				{
					if (APlayerController* PC = *PCIterator)
					{
						if (UMiraiControllerComponent_CharacterParts* CosmeticComponent = PC->FindComponentByClass<UMiraiControllerComponent_CharacterParts>())
						{
							CosmeticComponent->ApplyDeveloperSettings();
						}
					}
				}
			}));
	}
#endif	// WITH_SERVER_CODE
}

void UMiraiCosmeticDeveloperSettings::OnPlayInEditorStarted() const
{
	// Show a notification toast to remind the user that there's an experience override set
	if (CheatCosmeticCharacterParts.Num() > 0)
	{
		FNotificationInfo Info(LOCTEXT("CosmeticOverrideActive", "Applying Cosmetic Override"));
		Info.ExpireDuration = 2.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE

