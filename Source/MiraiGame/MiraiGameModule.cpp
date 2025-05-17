// Copyright CtrlCtrlV, Inc. All Rights Reserved.

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