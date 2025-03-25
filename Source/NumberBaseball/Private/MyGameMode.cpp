// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberBaseball/Public/MyGameMode.h"
#include "Types.h"
#include "MyFuncLib.h"
#include "MyGameState.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "Algo/RandomShuffle.h"
#include "GameFramework/PlayerState.h"

// void AMyGameMode::CallBroadCastMessage(const FText& Message)
// {
// 	auto MyGS = GetGameState<AMyGameState>();
// 	if (!MyGS)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("GameStart: GameState is null"));
// 		return;
// 	}
//
// 	MyGS->BroadCastMessage(Message);
// }

AMyGameMode::AMyGameMode()
{
	CurrentTurnIdx = -1;
	DefaultMaxTurn = 3;
	MaxTurn = DefaultMaxTurn;
}

void AMyGameMode::BroadcastMessage(const FText& Message)
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (auto PC = It->Get())
		{
			if (auto TargetPC = Cast<AMyPlayerController>(PC))
			{
				TargetPC->ClientUpdateMessage(Message);
			}
		}
	}
}

void AMyGameMode::CheckClientsReadyState()
{
	auto MyGS = GetGameState<AMyGameState>();
	if (!MyGS)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckClientsReadyState: GameState is null"));
		return;
	}

	auto PSArray = MyGS->PlayerArray;

	for (auto PS : PSArray)
	{
		if (auto TargetPS = Cast<AMyPlayerState>(PS))
		{
			if (TargetPS->GetUserState() != EUserState::Ready) return;
		}
	}

	GameStart();
}

void AMyGameMode::GameStart()
{
	// GameState Check
	auto MyGS = GetGameState<AMyGameState>();
	if (!MyGS)
	{
		UE_LOG(LogTemp, Error, TEXT("GameStart: GameState is null"));
		return;
	}

	// SetGameState
	MyGS->ServerSetIsInGame(true);

	// Player's Order, Turn Variables Setting
	for (auto TargetPS : MyGS->PlayerArray)
	{
		OrderOfPlayers.Add(TargetPS->GetPlayerId());
	}
	Algo::RandomShuffle(OrderOfPlayers);
	CurrentTurnIdx = -1;
	
	// Server Ball Value Setting
	ServerBall = UMyFuncLib::GenerateRandomNumberString();

	MaxTurn = DefaultMaxTurn;

	// SetPlayers
	SetPlayersTurn(true);

	// Game Start Text BroadCast
	BroadcastMessage(FText::FromString(TEXT("Game Start!!")));

	// MyGS->BroadCastMessage(FText::FromString(TEXT("테스트메세지")));
}

void AMyGameMode::CompareBall(AMyPlayerController* TargetPC, const FString& NumberMessage)
{
	FString PlayerBall = "";
	PlayerBall.AppendChar(NumberMessage[1]);
	PlayerBall.AppendChar(NumberMessage[2]);
	PlayerBall.AppendChar(NumberMessage[3]);

	UE_LOG(LogTemp, Log, TEXT("MaxTurn: %d"), MaxTurn);
	UE_LOG(LogTemp, Warning, TEXT("ServerBall: %s"), *ServerBall);
	UE_LOG(LogTemp, Warning, TEXT("PlayerBall: %s"), *PlayerBall);

	FString Result = UMyFuncLib::GetBallResult(ServerBall, PlayerBall);

	auto MyGS = GetGameState<AMyGameState>();
	if (!MyGS)
	{
		UE_LOG(LogTemp, Error, TEXT("CompareBall: GameState is null"));
		return;
	}
	
	MyGS->MulticastUpdatePlayerHistory(PlayerBall, Result);
	
	if (Result == TEXT("3S0B"))
	{
		if (auto TargetPS = Cast<AMyPlayerState>(TargetPC->PlayerState))
		{
			GameEnd(TargetPS->GetUsername());
			MyGS->ServerAddPlayerScore(TargetPS->GetUsername());
			return;
		}
	}
	SetPlayersTurn(false);
}

void AMyGameMode::AddPlayer(const FName& PlayerName)
{
	auto MyGS = Cast<AMyGameState>(GameState);
	MyGS->ServerAddPlayer(PlayerName);
	MyGS->MulticastUpdatePlayerScore();
}


void AMyGameMode::SetPlayersTurn(bool bIsAtGameStart)
{
	CurrentTurnIdx = (CurrentTurnIdx + 1) % GetWorld()->GetNumPlayerControllers();
	if (CurrentTurnIdx == 0) --MaxTurn;
	
	if (MaxTurn < 0)
	{
		GameEnd(FName());
		return;
	}

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (auto PC = It->Get())
		{
			if (auto TargetPC = Cast<AMyPlayerController>(PC))
			{
				if (auto TargetPS = Cast<AMyPlayerState>(TargetPC->PlayerState))
				{
					if (bIsAtGameStart) TargetPC->ClientClearHistoryBox();
					int32 CurrentTurnPlayerId = OrderOfPlayers[CurrentTurnIdx];
					if (CurrentTurnPlayerId == TargetPS->GetPlayerId())
					{
						TargetPS->ServerSetUserState(EUserState::Gaming_MyTurn);
						TargetPC->ClientUpdateUI(true, EUserState::Gaming_MyTurn);
					}
					else
					{
						TargetPS->ServerSetUserState(EUserState::Gaming_OtherTurn);
						TargetPC->ClientUpdateUI(true, EUserState::Gaming_OtherTurn);
					}
				}
			}
		}
	}
}

void AMyGameMode::SetPlayersToWaitState()
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (auto PC = It->Get())
		{
			if (auto TargetPC = Cast<AMyPlayerController>(PC))
			{
				if (auto TargetPS = Cast<AMyPlayerState>(TargetPC->PlayerState))
				{
					TargetPS->ServerSetUserState(EUserState::Waiting);
					TargetPC->ClientUpdateUI(false, EUserState::Waiting);
				}
			}
		}
	}
}

void AMyGameMode::GameEnd(const FName& WinnerName)
{
	// GameState Check
	auto MyGS = GetGameState<AMyGameState>();
	if (!MyGS)
	{
		UE_LOG(LogTemp, Error, TEXT("GameStart: GameState is null"));
		return;
	}

	// SetGameState
	MyGS->ServerSetIsInGame(false);

	// Broadcast Game Result Message
	if (WinnerName == NAME_None)
	{
		BroadcastMessage(FText::FromString(TEXT("승자 없음.. 게임 끝..")));
	}
	else
	{
		BroadcastMessage(FText::FromString(WinnerName.ToString() + TEXT(" 승리!")));
	}

	MyGS->MulticastUpdatePlayerScore();
	SetPlayersToWaitState();
}
