// Copyright CtrlCtrlV, Inc. All Rights Reserved.


#include "MiraiLogChannels.h"

#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogMirai);
DEFINE_LOG_CATEGORY(LogMiraiExperience);
DEFINE_LOG_CATEGORY(LogMiraiAbilitySystem);
DEFINE_LOG_CATEGORY(LogMiraiTeams);
DEFINE_LOG_CATEGORY(LogMiraiAnimation);
DEFINE_LOG_CATEGORY(LogMiraiCharacter);
DEFINE_LOG_CATEGORY(LogMiraiComponents);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("[]");
}
