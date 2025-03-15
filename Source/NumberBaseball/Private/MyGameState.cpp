// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"

void AMyGameState::BroadCastMessage_Implementation(const FText& Message)
{
	if (GEngine && HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10, FColor::White, Message.ToString());
	}

	// 다중출력됨. 왜?
	// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::White, Message.ToString());
}
