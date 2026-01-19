// Copyright(c) 2025, dvolkov.All rights reserved.

#include "MiraiWeaponUserInterface.h"

//#include "Equipment/MiraiEquipmentManagerComponent.h"
#include "GameFramework/Pawn.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(MiraiWeaponUserInterface)

struct FGeometry;

UMiraiWeaponUserInterface::UMiraiWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMiraiWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMiraiWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMiraiWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

