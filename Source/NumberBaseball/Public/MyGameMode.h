// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

class AMyPlayerController;
/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	// UI 업데이트에는 부적절
	// UFUNCTION()
	// void CallBroadCastMessage(const FText& Message);
	AMyGameMode();

	void BroadcastMessage(const FText& Message);
	void CheckClientsReadyState();
	void GameStart();
	void CompareBall(AMyPlayerController* TargetPC, const FString& NumberMessage);
	void AddPlayer(const FName& PlayerName);
	void SetPlayersTurn(bool bIsAtGameStart);
	void SetPlayersToWaitState();
	void GameEnd(const FName& WinnerName);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Server")
	TArray<int32> OrderOfPlayers;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Server")
	int32 CurrentTurnIdx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Server")
	FString ServerBall;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Server")
	int32 MaxTurn;
};
