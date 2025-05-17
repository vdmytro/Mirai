// Copyright CtrlCtrlV, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMirai, Log, All);
MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMiraiExperience, Log, All);
MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMiraiAbilitySystem, Log, All);
MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMiraiTeams, Log, All);
MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMiraiAnimation, Log, All);
MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMiraiCharacter, Log, All);
MIRAIGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogMiraiComponents, Log, All);

MIRAIGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
