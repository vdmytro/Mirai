// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "System/GameplayTagStack.h"
//#include "Teams/MiraiTeamAgentInterface.h"

#include "MiraiPlayerState.generated.h"

struct FMiraiVerbMessage;

class AController;
class AMiraiPlayerController;
class APlayerState;
class FName;
class UAbilitySystemComponent;
class UMiraiAbilitySystemComponent;
class UMiraiExperienceDefinition;
class UMiraiPawnData;
class UObject;
struct FFrame;
struct FGameplayTag;

/** Defines the types of client connected */
UENUM()
enum class EMiraiPlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};

/**
 * AMiraiPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class MIRAIGAME_API AMiraiPlayerState : public AModularPlayerState, public IAbilitySystemInterface/*, public IMiraiTeamAgentInterface*/
{
	GENERATED_BODY()

public:
	AMiraiPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Mirai|PlayerState")
	AMiraiPlayerController* GetMiraiPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Mirai|PlayerState")
	UMiraiAbilitySystemComponent* GetMiraiAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UMiraiPawnData* InPawnData);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface

	//~IMiraiTeamAgentInterface interface
	//virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	//virtual FGenericTeamId GetGenericTeamId() const override;
	//virtual FOnMiraiTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IMiraiTeamAgentInterface interface

	static const FName NAME_MiraiAbilityReady;

	void SetPlayerConnectionType(EMiraiPlayerConnectionType NewType);
	EMiraiPlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

	/** Returns the Squad ID of the squad the player belongs to. */
	//UFUNCTION(BlueprintCallable)
	//int32 GetSquadId() const
	//{
	//	return MySquadID;
	//}

	/** Returns the Team ID of the team the player belongs to. */
	//UFUNCTION(BlueprintCallable)
	//int32 GetTeamId() const
	//{
	//	return GenericTeamIdToInteger(MyTeamID);
	//}

	//void SetSquadID(int32 NewSquadID);

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "Mirai|PlayerState")
	void ClientBroadcastMessage(const FMiraiVerbMessage Message);

	// Gets the replicated view rotation of this player, used for spectating
	FRotator GetReplicatedViewRotation() const;

	// Sets the replicated view rotation, only valid on the server
	void SetReplicatedViewRotation(const FRotator& NewRotation);

private:
	void OnExperienceLoaded(const UMiraiExperienceDefinition* CurrentExperience);

protected:
	UFUNCTION()
	void OnRep_PawnData();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UMiraiPawnData> PawnData;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Mirai|PlayerState")
	TObjectPtr<UMiraiAbilitySystemComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	//UPROPERTY()
	//TObjectPtr<const class UMiraiHealthSet> HealthSet;
	// Combat attribute set used by this actor.
	//UPROPERTY()
	//TObjectPtr<const class UMiraiCombatSet> CombatSet;

	UPROPERTY(Replicated)
	EMiraiPlayerConnectionType MyPlayerConnectionType;

	//UPROPERTY()
	//FOnMiraiTeamIndexChangedDelegate OnTeamChangedDelegate;

	//UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	//FGenericTeamId MyTeamID;

	//UPROPERTY(ReplicatedUsing=OnRep_MySquadID)
	//int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	FRotator ReplicatedViewRotation;

private:
	//UFUNCTION()
	//void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	//UFUNCTION()
	//void OnRep_MySquadID();
};
