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

	AddPlayerToServer(FName(NewUsername.ToString()));
}

void AMyPlayerController::UpdateScoreBox()
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

	UE_LOG(LogTemp, Warning, TEXT("PC->UpdateScoreBox: Run Widget->UpdateScoreBox"));
	ChatWidget->UpdateScoreBox();
}

void AMyPlayerController::AddPlayerToServer_Implementation(const FName& PlayerName)
{
	auto MyGM = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("AddPlayerToServer: No GameMode"));
		return;
	}

	MyGM->AddPlayer(PlayerName);	
}

void AMyPlayerController::ClearHistoryBoxClient_Implementation()
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SendOneBallResultToClient: Null MyHUD"));
		return;
	}

	auto HistoryBox = ChatWidget->HistoryVerticalBox;
	HistoryBox->ClearChildren();
}

void AMyPlayerController::SendOneBallResultToClient_Implementation(const FString& PlayerString,
                                                                   const FString& ResultString)
{
	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyHUD);
	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SendOneBallResultToClient: Null MyHUD"));
		return;
	}

	auto HistoryBox = ChatWidget->HistoryVerticalBox;
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(HistoryBox);
	NewTextBlock->SetText(FText::FromString(PlayerString + TEXT(" -> ") + ResultString));
	HistoryBox->AddChild(NewTextBlock);
}

void AMyPlayerController::UpdateMessageToClient_Implementation(const FText& Message)
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

	UVerticalBox* ChatBox = ChatWidget->ChatVerticalBox;
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatBox);
	NewTextBlock->SetText(Message);
	ChatBox->AddChild(NewTextBlock);
}

void AMyPlayerController::SendMessageToServer_Implementation(AMyPlayerController* MyPC, const FText& Message)
{
	auto MyGM = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("SendMessageToServer: Null GameMode"));
		return;
	}

	MyGM->BroadcastMessage(Message);
}


void AMyPlayerController::SendBaseballMessageToServer_Implementation(AMyPlayerController* MyPC,
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

void AMyPlayerController::UpdateUIToClient_Implementation(const bool bIsInGame, const EUserState& UserState)
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

void AMyPlayerController::SendReadySignalToServer_Implementation()
{
	auto MyGM = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (!MyGM)
	{
		UE_LOG(LogTemp, Error, TEXT("SendReadySignalToServer: Null GameMode"));
		return;
	}

	MyGM->CheckClientsReadyState();
}
