// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiAimSensitivityData.h"

#include "Settings/MiraiSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiAimSensitivityData)

UMiraiAimSensitivityData::UMiraiAimSensitivityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SensitivityMap =
	{
		{ EMiraiGamepadSensitivity::Slow,			0.5f },
		{ EMiraiGamepadSensitivity::SlowPlus,		0.75f },
		{ EMiraiGamepadSensitivity::SlowPlusPlus,	0.9f },
		{ EMiraiGamepadSensitivity::Normal,		1.0f },
		{ EMiraiGamepadSensitivity::NormalPlus,	1.1f },
		{ EMiraiGamepadSensitivity::NormalPlusPlus,1.25f },
		{ EMiraiGamepadSensitivity::Fast,			1.5f },
		{ EMiraiGamepadSensitivity::FastPlus,		1.75f },
		{ EMiraiGamepadSensitivity::FastPlusPlus,	2.0f },
		{ EMiraiGamepadSensitivity::Insane,		2.5f },
	};
}

const float UMiraiAimSensitivityData::SensitivtyEnumToFloat(const EMiraiGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}

