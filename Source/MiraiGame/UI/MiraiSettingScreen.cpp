// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiSettingScreen.h"

#include "Input/CommonUIInputTypes.h"
#include "Player/MiraiLocalPlayer.h"
#include "Settings/MiraiGameSettingRegistry.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiSettingScreen)

class UGameSettingRegistry;

void UMiraiSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UMiraiSettingScreen::CreateRegistry()
{
	UMiraiGameSettingRegistry* NewRegistry = NewObject<UMiraiGameSettingRegistry>();

	if (UMiraiLocalPlayer* LocalPlayer = CastChecked<UMiraiLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void UMiraiSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void UMiraiSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void UMiraiSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void UMiraiSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
