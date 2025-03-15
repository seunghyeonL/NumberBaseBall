// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
	bIsNameSet = false;
	Username = TEXT("Anonymous");
}

void AMyPlayerState::Login(const FText& NewUsername)
{
	bIsNameSet = true;
	Username = FName(NewUsername.ToString());
}
