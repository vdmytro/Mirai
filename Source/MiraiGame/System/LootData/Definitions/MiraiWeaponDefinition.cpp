// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/LootData/Definitions/MiraiWeaponDefinition.h"

uint32 GetTypeHash(const FWeaponAttachmentSlot& obj)
{
    return FCrc::MemCrc32(&obj, sizeof(FWeaponAttachmentSlot));
}
