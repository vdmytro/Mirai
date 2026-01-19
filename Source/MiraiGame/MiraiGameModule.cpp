// Copyright (c) 2025, dvolkov. All rights reserved.

#include "Modules/ModuleManager.h"


/**
 * FMiraiGameModule
 */
class FMiraiGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FMiraiGameModule, MiraiGame, "MiraiGame");