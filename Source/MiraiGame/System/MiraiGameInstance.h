// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonGameInstance.h"

#include "MiraiGameInstance.generated.h"

class AMiraiPlayerController;
class UObject;

UCLASS(Config = Game)
class MIRAIGAME_API UMiraiGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

// UMiraiGameInstance
public:
	UMiraiGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	AMiraiPlayerController* GetPrimaryPlayerController() const;
	virtual bool CanJoinRequestedSession() const override;
	virtual void HandlerUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext) override;
	virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
	virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	void OnPreClientTravelToSession(FString& URL);
private:

//Properties
public:
protected:
	/** A hard-coded encryption key used to try out the encryption code. This is NOT SECURE, do not use this technique in production! */
	TArray<uint8> DebugTestEncryptionKey;
private:

};
