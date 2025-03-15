// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberBaseball/Public/MyPlayerController.h"

#include "MyGameState.h"
#include "NumberBaseball/Public/MyPlayerHUD.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerController::SendMessageToServer_Implementation(const FText& Message)
{
	if (auto MyGS = GetWorld()->GetGameState<AMyGameState>())
	{
		MyGS->BroadCastMessage(Message);
	}
}


