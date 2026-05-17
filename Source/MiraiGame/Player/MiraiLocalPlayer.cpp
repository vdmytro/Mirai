// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MiraiLocalPlayer.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Settings/MiraiSettingsLocal.h"
#include "Settings/MiraiSettingsShared.h"
#include "CommonUserSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiLocalPlayer)

class UObject;

UMiraiLocalPlayer::UMiraiLocalPlayer()
{
}

void UMiraiLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (UMiraiSettingsLocal* LocalSettings = GetLocalSettings())
	{
		LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &UMiraiLocalPlayer::OnAudioOutputDeviceChanged);
	}
}

void UMiraiLocalPlayer::SwitchController(class APlayerController* PC)
{
	Super::SwitchController(PC);

	OnPlayerControllerChanged(PlayerController);
}

bool UMiraiLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void UMiraiLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);

	Super::InitOnlineSession();
}

void UMiraiLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	// Stop listening for changes from the old controller
	//FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	//if (IMiraiTeamAgentInterface* ControllerAsTeamProvider = Cast<IMiraiTeamAgentInterface>(LastBoundPC.Get()))
	//{
	//	OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
	//	ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	//}

	// Grab the current team ID and listen for future changes
	//FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	//if (IMiraiTeamAgentInterface* ControllerAsTeamProvider = Cast<IMiraiTeamAgentInterface>(NewController))
	//{
	//	NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
	//	ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	//	LastBoundPC = NewController;
	//}

	//ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

//void UMiraiLocalPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
//{
//	// Do nothing, we merely observe the team of our associated player controller
//}

//FGenericTeamId UMiraiLocalPlayer::GetGenericTeamId() const
//{
//	if (IMiraiTeamAgentInterface* ControllerAsTeamProvider = Cast<IMiraiTeamAgentInterface>(PlayerController))
//	{
//		return ControllerAsTeamProvider->GetGenericTeamId();
//	}
//	else
//	{
//		return FGenericTeamId::NoTeam;
//	}
//}

//FOnMiraiTeamIndexChangedDelegate* UMiraiLocalPlayer::GetOnTeamIndexChangedDelegate()
//{
//	return &OnTeamChangedDelegate;
//}

UMiraiSettingsLocal* UMiraiLocalPlayer::GetLocalSettings() const
{
	return UMiraiSettingsLocal::Get();
}

UMiraiSettingsShared* UMiraiLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		// On PC it's okay to use the sync load because it only checks the disk
		// This could use a platform tag to check for proper save support instead
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
		
		if (bCanLoadBeforeLogin)
		{
			SharedSettings = UMiraiSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			// We need to wait for user login to get the real settings so return temp ones
			SharedSettings = UMiraiSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}

void UMiraiLocalPlayer::LoadSharedSettingsFromDisk(bool bForceLoad)
{
	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
	{
		// Already loaded once, don't reload
		return;
	}

	ensure(UMiraiSettingsShared::AsyncLoadOrCreateSettings(this, UMiraiSettingsShared::FOnSettingsLoadedEvent::CreateUObject(this, &UMiraiLocalPlayer::OnSharedSettingsLoaded)));
}

void UMiraiLocalPlayer::OnSharedSettingsLoaded(UMiraiSettingsShared* LoadedOrCreatedSettings)
{
	// The settings are applied before it gets here
	if (ensure(LoadedOrCreatedSettings))
	{
		// This will replace the temporary or previously loaded object which will GC out normally
		SharedSettings = LoadedOrCreatedSettings;

		NetIdForSharedSettings = GetCachedUniqueNetId();
	}
}

void UMiraiLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
	FOnCompletedDeviceSwap DevicesSwappedCallback;
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
}

void UMiraiLocalPlayer::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
	if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
	{
	}
}

//void UMiraiLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
//{
//	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
//}

