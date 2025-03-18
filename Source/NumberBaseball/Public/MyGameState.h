// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API AMyGameState : public AGameState
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated)
	bool bIsInGame;

	UPROPERTY(Replicated)
	TArray<FPlayerScoreData> PlayerScoreDatas;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	bool GetIsInGame() const { return bIsInGame; }
	TArray<FPlayerScoreData> GetPlayerScoreDatas() const { return PlayerScoreDatas; }	
	
	UFUNCTION(Server, Reliable)
	void SetIsInGame(bool IsInGame);

	UFUNCTION(Server, Reliable)
	void AddPlayer(const FName& PlayerName);

	UFUNCTION(Server, Reliable)
	void AddPlayerScore(const FName& PlayerName);

	UFUNCTION(NetMulticast, Reliable)
	void UpdatePlayerScoreMulticast();

	// UFUNCTION(NetMulticast, Reliable)
	// void BroadCastMessage(const FText& Message);

	// UI 업데이트에는 부적절
	// UFUNCTION(NetMulticast, Reliable)
	// void BroadCastMessage(const FText& Message);
};
