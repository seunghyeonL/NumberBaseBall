// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class NUMBERBASEBALL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();
	
	virtual void BeginPlay() override;
	
	void Login(const FText& NewUsername);

	UFUNCTION(Server, Reliable)
	void ServerAddPlayer(const FName& PlayerName);

	UFUNCTION(Server, Reliable)
	void ServerSendMessage(AMyPlayerController* MyPC, const FText& Message);

	UFUNCTION(Server, Reliable)
	void ServerSendBaseballMessage(AMyPlayerController* MyPC, const FString& NumberMessage);

	UFUNCTION(Server, Reliable)
	void ServerSendReadySignal();

	UFUNCTION(Client, Reliable)
	void ClientUpdateMessage(const FText& Message);

	UFUNCTION(Client, Reliable)
	void ClientUpdateUI(const bool bIsInGame, const EUserState& UserState);
	
	// UFUNCTION(Client, Reliable)
	void ClientSendOneBallResult(const FString& PlayerString, const FString& ResultString);

	UFUNCTION(Client, Reliable)
	void ClientClearHistoryBox();

	// void UpdateScoreBox(const TArray<FPlayerScoreData>& PlayerScores);
	UFUNCTION(Client, Reliable)
	void ClientUpdateScoreBox();
};
