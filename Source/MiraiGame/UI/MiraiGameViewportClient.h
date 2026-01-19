// Copyright(c) 2025, dvolkov.All rights reserved.

#pragma once

#include "CommonGameViewportClient.h"

#include "MiraiGameViewportClient.generated.h"

class UGameInstance;
class UObject;

UCLASS(BlueprintType)
class UMiraiGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	UMiraiGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
