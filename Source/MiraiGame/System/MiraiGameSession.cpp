// Copyright(c) 2025, dvolkov.All rights reserved.

#include "MiraiGameSession.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiGameSession)


AMiraiGameSession::AMiraiGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AMiraiGameSession::ProcessAutoLogin()
{
	// This is actually handled in MiraiGameMode::TryDedicatedServerLogin
	return true;
}

void AMiraiGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AMiraiGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

