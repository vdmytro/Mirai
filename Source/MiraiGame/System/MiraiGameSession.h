// Copyright(c) 2025, dvolkov.All rights reserved.

#pragma once

#include "GameFramework/GameSession.h"

#include "MiraiGameSession.generated.h"

class UObject;


UCLASS(Config = Game)
class AMiraiGameSession : public AGameSession
{
	GENERATED_BODY()

// AMiraiGameSession
public:
	AMiraiGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	/** Override to disable the default behavior */
	virtual bool ProcessAutoLogin() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
private:

//Properties
public:
protected:
private:
};
