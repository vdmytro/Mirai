// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "ModularGameState.h"

#include "MiraiGameState.generated.h"

struct FMiraiVerbMessage;

class APlayerState;
class UAbilitySystemComponent;
class UMiraiAbilitySystemComponent;
class UMiraiExperienceManagerComponent;
class UObject;
struct FFrame;

/**
 * AMiraiGameState
 *
 *	The base game state class used by this project.
 */
UCLASS(Config = Game)
class MIRAIGAME_API AMiraiGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AMiraiGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	//~End of AActor interface

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//~End of AGameStateBase interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	// Gets the ability system component used for game wide things
	UFUNCTION(BlueprintCallable, Category = "Mirai|GameState")
	UMiraiAbilitySystemComponent* GetMiraiAbilitySystemComponent() const { return AbilitySystemComponent; }

	// Send a message that all clients will (probably) get
	// (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "Mirai|GameState")
	void MulticastMessageToClients(const FMiraiVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Mirai|GameState")
	void MulticastReliableMessageToClients(const FMiraiVerbMessage Message);

	// Gets the server's FPS, replicated to clients
	float GetServerFPS() const;

	// Indicate the local player state is recording a replay
	void SetRecorderPlayerState(APlayerState* NewPlayerState);

	// Gets the player state that recorded the replay, if valid
	APlayerState* GetRecorderPlayerState() const;

	// Delegate called when the replay player state changes
	//DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecorderPlayerStateChanged, APlayerState*);
	//FOnRecorderPlayerStateChanged OnRecorderPlayerStateChangedEvent;

private:
	// Handles loading and managing the current gameplay experience
	UPROPERTY()
	TObjectPtr<UMiraiExperienceManagerComponent> ExperienceManagerComponent;

	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "Mirai|GameState")
	TObjectPtr<UMiraiAbilitySystemComponent> AbilitySystemComponent;

protected:
	UPROPERTY(Replicated)
	float ServerFPS;

	// The player state that recorded a replay, it is used to select the right pawn to follow
	// This is only set in replay streams and is not replicated normally
	//UPROPERTY(Transient, ReplicatedUsing = OnRep_RecorderPlayerState)
	//TObjectPtr<APlayerState> RecorderPlayerState;

	//UFUNCTION()
	//void OnRep_RecorderPlayerState();

};
