// Fill out your copyright notice in the Description page of Project Settings. 

#include "MyGameState.h"
#include "MyPlayerController.h"
#include "Net/UnrealNetwork.h"

// 서버 에서 Multicast RPC 호출
// -> 모든 클라이언트에서 함수 실행

// void AMyGameState::BroadCastMessage_Implementation(const FText& Message)
// {
// 	// if (HasAuthority())
// 	// {
// 	// 	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10, FColor::White, Message.ToString());
// 	// }
// 	// 메세지    메세지    메세지
// 	// 클라에서 실행하면 
// 	// 다중출력됨. 왜?
// 	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::White, Message.ToString());
// 	// GameMode 에서 실행
// 	// 서버  -> 1 띄우고 2 띄우고 3 띄우고
// 	// 클라1 -> 1 띄우고 2 띄우고 3 띄우고
// 	// 클라2 -> 1 띄우고 2 띄우고 3 띄우고
// 	//
// 	//    메세지    메세지    메세지
// 	//    메세지    메세지    메세지
// 	//    메세지    메세지    메세지
// }

void AMyGameState::SetIsInGame_Implementation(bool IsInGame)
{
	bIsInGame = IsInGame;
}

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameState, bIsInGame);
}

void AMyGameState::UpdatePlayerScoreMulticast_Implementation()
{
	auto MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!MyPC)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdatePlayerScoreMulticast: AMyPlayerController is NULL"));
		return;
	}

	MyPC->UpdateScoreBox();
}

void AMyGameState::AddPlayer_Implementation(const FName& PlayerName)
{
	PlayerScoreDatas.Add({PlayerName, 0});
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// UE_LOG(LogTemp, Warning, TEXT("AddPlayer: Before PlayerController Exists."));
		if (auto PC = It->Get())
		{
			// UE_LOG(LogTemp, Warning, TEXT("AddPlayer: PlayerController Exists."));
			if (auto TargetPC = Cast<AMyPlayerController>(PC))
			{
				UE_LOG(LogTemp, Warning, TEXT("AddPlayer: MyPlayerController Exists."));
				// TargetPC->UpdateScoreBox(PlayerScoreDatas);
				TargetPC->UpdateScoreBox();
			}
		}
	}
}

void AMyGameState::AddPlayerScore_Implementation(const FName& PlayerName)
{
	for (auto& PlayerScoreData : PlayerScoreDatas)
	{
		if (PlayerScoreData.PlayerName == PlayerName)
		{
			PlayerScoreData.PlayerScore++;
		}
	}
}


