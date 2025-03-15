// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberBaseball/Public/MyPlayerHUD.h"

AMyPlayerHUD::AMyPlayerHUD()
{
	ChatWidgetClass = nullptr;
	ChatWidgetInstance = nullptr;
}

void AMyPlayerHUD::ShowChatWidget()
{
	if (ChatWidgetClass)
	{
		ChatWidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), ChatWidgetClass);
		if (ChatWidgetInstance)
		{
			ChatWidgetInstance->AddToViewport();
		}
	}
}

void AMyPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowChatWidget();
}


