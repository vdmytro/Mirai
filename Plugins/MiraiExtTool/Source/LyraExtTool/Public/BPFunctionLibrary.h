// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "BPFunctionLibrary.generated.h"

class UMaterialInterface;
class UObject;
class UStaticMesh;
struct FFrame;

/**
 *
 */

UCLASS()
class UBPFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category="MiraiExt")
    static bool ChangeMeshMaterials(TArray<UStaticMesh*> Mesh, UMaterialInterface* Material);
};
