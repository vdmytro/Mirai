// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModularAIController.h"
//#include "Teams/MiraiTeamAgentInterface.h"

#include "MiraiPlayerBotController.generated.h"

//namespace ETeamAttitude { enum Type : int; }
//struct FGenericTeamId;

class APlayerState;
class UAIPerceptionComponent;
class UObject;
struct FFrame;

/**
 * AMiraiPlayerBotController
 *
 *	The controller class used by player bots in this project.
 */
UCLASS(Blueprintable)
class AMiraiPlayerBotController : public AModularAIController/*, public IMiraiTeamAgentInterface*/
{
	GENERATED_BODY()

public:
	AMiraiPlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IMiraiTeamAgentInterface interface
	//virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	//virtual FGenericTeamId GetGenericTeamId() const override;
	//virtual FOnMiraiTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End of IMiraiTeamAgentInterface interface

	// Attempts to restart this controller (e.g., to respawn it)
	void ServerRestartController();

	//Update Team Attitude for the AI
	//UFUNCTION(BlueprintCallable, Category = "Mirai AI Player Controller")
	//void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);

	virtual void OnUnPossess() override;


private:
	//UFUNCTION()
	//void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

private:
	void BroadcastOnPlayerStateChanged();

protected:	
	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

private:
	//UPROPERTY()
	//FOnMiraiTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
