// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberBaseball/Public/MyPlayerController.h"

#include "MyChatWidget.h"
#include "MyGameMode.h"
#include "MyGameState.h"
#include "MyPlayerState.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "NumberBaseball/Public/MyPlayerHUD.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerController::Login(const FText& NewUsername)
{
	auto MyPS = Cast<AMyPlayerState>(PlayerState);
	if (!MyPS)
	{
		UE_LOG(LogTemp, Error, TEXT("Login: No PlayerState"));
		return;
	}

	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Login: ChatWidget is NULL"));
		return;
	}

	MyPS->ServerSetUserState(EUserState::Waiting);
	MyPS->ServerSetUsername(FName(NewUsername.ToString()));

	ChatWidget->UpdateUserStateText(EUserState::Waiting);

	ServerAddPlayer(FName(NewUsername.ToString()));
}

void AMyPlayerController::ClientUpdateScoreBox_Implementation()
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	if (!MyPlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateScoreBox: No PlayerHUD"));
		return;
	}
	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateScoreBox: ChatWidget is NULL"));
		return;
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("PC->UpdateScoreBox: Run Widget->UpdateScoreBox"));

	ChatWidget->UpdateScoreBox();
}

void AMyPlayerController::ServerAddPlayer_Implementation(const FName& PlayerName)
{
	auto MyGM = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("AddPlayerToServer: No GameMode"));
		return;
	}

	MyGM->AddPlayer(PlayerName);	
}

void AMyPlayerController::ClientClearHistoryBox_Implementation()
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SendOneBallResultToClient: Null MyHUD"));
		return;
	}

	ChatWidget->ClearHistoryBox();
}

void AMyPlayerController::ClientSendOneBallResult(const FString& PlayerString,
                                                                   const FString& ResultString)
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SendOneBallResultToClient: Null MyHUD"));
		return;
	}

	ChatWidget->AddHistoryTextBlock(PlayerString, ResultString);
}

void AMyPlayerController::ClientUpdateMessage_Implementation(const FText& Message)
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	if (!MyPlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateMessageToClient: Null MyHUD"));
		return;
	}

	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateMessageToClient: Null ChatWidget"));
		return;
	}

	ChatWidget->AddChatTextBlock(Message);
}

void AMyPlayerController::ServerSendMessage_Implementation(AMyPlayerController* MyPC, const FText& Message)
{
	auto MyGM = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("SendMessageToServer: Null GameMode"));
		return;
	}

	MyGM->BroadcastMessage(Message);
}


void AMyPlayerController::ServerSendBaseballMessage_Implementation(AMyPlayerController* MyPC,
                                                                     const FString& NumberMessage)
{
	auto MyGM = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("SendBaseballMessageToServer: Null GameMode"));
		return;
	}

	MyGM->CompareBall(MyPC, NumberMessage);
}

void AMyPlayerController::ClientUpdateUI_Implementation(const bool bIsInGame, const EUserState& UserState)
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	if (!MyPlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateUIToClient: Null MyHUD"));
		return;
	}

	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateUIToClient: Null ChatWidget"));
		return;
	}

	auto MyPS = Cast<AMyPlayerState>(PlayerState);

	ChatWidget->UpdateReadyButtonState(bIsInGame);
	ChatWidget->UpdateUserStateText(UserState);
}

void AMyPlayerController::ServerSendReadySignal_Implementation()
{
	auto MyGM = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("SendReadySignalToServer: Null GameMode"));
		return;
	}

	MyGM->CheckClientsReadyState();
}
