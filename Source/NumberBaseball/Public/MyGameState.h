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
	TArray<FPlayerScoreData> PlayerScoreDatas; // 바꿨어
	// 값이 변경될때 OnRep 호출하고
	// 클라 두개에만 UpdateScore

	FTimerHandle ScoreTimerHandle;

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

	UFUNCTION(NetMulticast, Reliable)
	void UpdatePlayerHistoryMulticast(const FString& PlayerString,
	                                  const FString& ResultString);

	void UpdatePlayerScoreMulticastTimerHandler();

	// 서버 호출
	// 클라1 실행

	// 1. A 함수를 서버가 가지고 와서 서버가 이거를 실행해줌 이거는 언리얼에서 안됨 X
	// 2. 이벤트 디스패처 방식 -> OnRep 
	// 3. 직접전달
	// 4. RPC 

	// UFUNCTION(NetMulticast, Reliable)
	// void BroadCastMessage(const FText& Message);

	// UI 업데이트에는 부적절
	// UFUNCTION(NetMulticast, Reliable)
	// void BroadCastMessage(const FText& Message);
};
