// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiraiWorldCollectable.h"

#include "Async/TaskGraphInterfaces.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiWorldCollectable)

struct FInteractionQuery;

AMiraiWorldCollectable::AMiraiWorldCollectable()
{
}

void AMiraiWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup AMiraiWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
