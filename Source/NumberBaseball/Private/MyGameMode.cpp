// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberBaseball/Public/MyGameMode.h"
#include "MyFuncLib.h"
#include "MyGameState.h"
#include "Algo/RandomShuffle.h"
#include "GameFramework/PlayerState.h"

void AMyGameMode::GameStart()
{
	// GameState Check
	auto MyGS = GetGameState<AMyGameState>();
	if (!MyGS)
	{
		UE_LOG(LogTemp, Error, TEXT("GameStart: GameState is null"));
		return;
	}

	// Player's Order, Turn Variables Setting
	for (auto TargetPS: MyGS->PlayerArray)
	{
		OrderOfPlayers.Add(TargetPS->GetUniqueId());
	}
	Algo::RandomShuffle(OrderOfPlayers);
	CurrentTurnIdx = 0;

	// Server Ball Value Setting
	ServerBall = UMyFuncLib::GenerateRandomNumberString();
}
