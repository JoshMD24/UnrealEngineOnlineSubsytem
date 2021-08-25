// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundFPSGameInstance.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

URoundFPSGameInstance::URoundFPSGameInstance()
{

}

void URoundFPSGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//Bind Delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URoundFPSGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URoundFPSGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URoundFPSGameInstance::OnJoinSessionComplete);
		}
	}
}

void URoundFPSGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeded: %d"), Succeded)
	if (Succeded)
	{
		GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/FirstPersonExampleMap?listen");
	}
}

void URoundFPSGameInstance::OnFindSessionComplete(bool Succeded)
{
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("OnFindSessionComplete"));
	if (Succeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("JoiningServer"));
			SessionInterface->JoinSession(0, "My Session", SearchResults[0]);
		}
	}
}

void URoundFPSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("OnJoinSessionComplete"));
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("Travel"));
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, SessionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, JoinAddress);
		}
	}
}

void URoundFPSGameInstance::CreateServer()
{
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 10;

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}

void URoundFPSGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}