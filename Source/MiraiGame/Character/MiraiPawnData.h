// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "MiraiPawnData.generated.h"

class APawn;
class UMiraiAbilitySet;
class UMiraiAbilityTagRelationshipMapping;
class UMiraiCameraMode;
class UMiraiInputConfig;
class UObject;


/**
 * UMiraiPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Mirai Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class MIRAIGAME_API UMiraiPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMiraiPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from AMiraiPawn or AMiraiCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mirai|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mirai|Abilities")
	TArray<TObjectPtr<UMiraiAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mirai|Abilities")
	TObjectPtr<UMiraiAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mirai|Input")
	TObjectPtr<UMiraiInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mirai|Camera")
	TSubclassOf<UMiraiCameraMode> DefaultCameraMode;
};
