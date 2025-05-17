// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiPlayerState.h"

//#include "AbilitySystem/Attributes/MiraiCombatSet.h"
//#include "AbilitySystem/Attributes/MiraiHealthSet.h"
#include "AbilitySystem/MiraiAbilitySet.h"
#include "AbilitySystem/MiraiAbilitySystemComponent.h"
#include "Character/MiraiPawnData.h"
#include "Character/MiraiPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
//#include "GameModes/MiraiExperienceManagerComponent.h"
//@TODO: Would like to isolate this a bit better to get the pawn data in here without this having to know about other stuff
#include "GameModes/MiraiGameMode.h"
#include "MiraiLogChannels.h"
#include "MiraiPlayerController.h"
#include "Messages/MiraiVerbMessage.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiPlayerState)

class AController;
class APlayerState;
class FLifetimeProperty;

const FName AMiraiPlayerState::NAME_MiraiAbilityReady("MiraiAbilitiesReady");

AMiraiPlayerState::AMiraiPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MyPlayerConnectionType(EMiraiPlayerConnectionType::Player)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMiraiAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	//HealthSet = CreateDefaultSubobject<UMiraiHealthSet>(TEXT("HealthSet"));
	//CombatSet = CreateDefaultSubobject<UMiraiCombatSet>(TEXT("CombatSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;

	//MyTeamID = FGenericTeamId::NoTeam;
	//MySquadID = INDEX_NONE;
}

void AMiraiPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMiraiPlayerState::Reset()
{
	Super::Reset();
}

void AMiraiPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (UMiraiPawnExtensionComponent* PawnExtComp = UMiraiPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void AMiraiPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	//@TODO: Copy stats
}

void AMiraiPlayerState::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
		case EMiraiPlayerConnectionType::Player:
		case EMiraiPlayerConnectionType::InactivePlayer:
			//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
			// (e.g., for long running servers where they might build up if lots of players cycle through)
			bDestroyDeactivatedPlayerState = true;
			break;
		default:
			bDestroyDeactivatedPlayerState = true;
			break;
	}
	
	SetPlayerConnectionType(EMiraiPlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void AMiraiPlayerState::OnReactivated()
{
	if (GetPlayerConnectionType() == EMiraiPlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(EMiraiPlayerConnectionType::Player);
	}
}

void AMiraiPlayerState::OnExperienceLoaded(const UMiraiExperienceDefinition* /*CurrentExperience*/)
{
	//if (AMiraiGameMode* MiraiGameMode = GetWorld()->GetAuthGameMode<AMiraiGameMode>())
	//{
	//	if (const UMiraiPawnData* NewPawnData = MiraiGameMode->GetPawnDataForController(GetOwningController()))
	//	{
	//		SetPawnData(NewPawnData);
	//	}
	//	else
	//	{
	//		UE_LOG(LogMirai, Error, TEXT("AMiraiPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
	//	}
	//}
}

void AMiraiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
	//DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	//DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);

	SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedViewRotation, SharedParams);

	DOREPLIFETIME(ThisClass, StatTags);	
}

FRotator AMiraiPlayerState::GetReplicatedViewRotation() const
{
	// Could replace this with custom replication
	return ReplicatedViewRotation;
}

void AMiraiPlayerState::SetReplicatedViewRotation(const FRotator& NewRotation)
{
	if (NewRotation != ReplicatedViewRotation)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ReplicatedViewRotation, this);
		ReplicatedViewRotation = NewRotation;
	}
}

AMiraiPlayerController* AMiraiPlayerState::GetMiraiPlayerController() const
{
	return Cast<AMiraiPlayerController>(GetOwner());
}

UAbilitySystemComponent* AMiraiPlayerState::GetAbilitySystemComponent() const
{
	return GetMiraiAbilitySystemComponent();
}

void AMiraiPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		//UMiraiExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UMiraiExperienceManagerComponent>();
		//check(ExperienceComponent);
		//ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnMiraiExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void AMiraiPlayerState::SetPawnData(const UMiraiPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogMirai, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UMiraiAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_MiraiAbilityReady);
	
	ForceNetUpdate();
}

void AMiraiPlayerState::OnRep_PawnData()
{
}

void AMiraiPlayerState::SetPlayerConnectionType(EMiraiPlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}

//void AMiraiPlayerState::SetSquadID(int32 NewSquadId)
//{
//	if (HasAuthority())
//	{
//		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MySquadID, this);
//
//		MySquadID = NewSquadId;
//	}
//}

//void AMiraiPlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
//{
//	if (HasAuthority())
//	{
//		const FGenericTeamId OldTeamID = MyTeamID;
//
//		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
//		MyTeamID = NewTeamID;
//		ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
//	}
//	else
//	{
//		UE_LOG(LogMiraiTeams, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
//	}
//}

//FGenericTeamId AMiraiPlayerState::GetGenericTeamId() const
//{
//	return MyTeamID;
//}

//FOnMiraiTeamIndexChangedDelegate* AMiraiPlayerState::GetOnTeamIndexChangedDelegate()
//{
//	return &OnTeamChangedDelegate;
//}

//void AMiraiPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
//{
//	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
//}

//void AMiraiPlayerState::OnRep_MySquadID()
//{
//	//@TODO: Let the squad subsystem know (once that exists)
//}

void AMiraiPlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void AMiraiPlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 AMiraiPlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool AMiraiPlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void AMiraiPlayerState::ClientBroadcastMessage_Implementation(const FMiraiVerbMessage Message)
{
	// This check is needed to prevent running the action when in standalone mode
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

