// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "RoundFPSGameInstance.generated.h"

UCLASS()
class ROUNDFPS_API URoundFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URoundFPSGameInstance();

protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeded);
	virtual void OnFindSessionComplete(bool Succeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
		void CreateServer();

	UFUNCTION(BlueprintCallable)
		void JoinServer();
};
