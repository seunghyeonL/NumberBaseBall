// Fill out your copyright notice in the Description page of Project Settings.

#include "MyChatWidget.h"

#include "MyGameState.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMyChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox)
	{
		EditableTextBox->OnTextCommitted.AddDynamic(this, &UMyChatWidget::OnTextCommitted);
	}

	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddDynamic(this, &UMyChatWidget::OnReadyButtonClicked);
	}
}

void UMyChatWidget::OnTextCommitted(const FText& CurrentText, ETextCommit::Type CommitType)
{
	if (CommitType != ETextCommit::OnEnter) return;

	const auto MyPC = GetOwningPlayer<AMyPlayerController>();
	if (!MyPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted: No Owning PlayerController."));
		return;
	}

	const auto MyPS = MyPC->GetPlayerState<AMyPlayerState>();
	if (!MyPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted: No Matching PlayerState."));
		return;
	}

	if (MyPS->GetUserState() == EUserState::BeforeLogin)
	{
		SendNameSettingChat(CurrentText, MyPS, MyPC);
	}
	else
	{
		if (CurrentText.ToString()[0] != '/')
		{
			SendNormalChat(CurrentText, MyPS, MyPC);
		}
		else
		{
			SendBaseballNumber(CurrentText, MyPS, MyPC);
		}
	}
}

void UMyChatWidget::OnReadyButtonClicked()
{
	// 플레이어들 준비완료 상태 체크하는
	const auto MyPC = GetOwningPlayer<AMyPlayerController>();
	if (!MyPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted: No Owning PlayerController."));
		return;
	}

	const auto MyPS = MyPC->GetPlayerState<AMyPlayerState>();
	if (!MyPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted: No Matching PlayerState."));
		return;
	}

	EUserState UserState = MyPS->GetUserState();
	if (UserState == EUserState::Waiting)
	{
		MyPS->ServerSetUserState(EUserState::Ready);
		UpdateUserStateText(EUserState::Ready);
	}
	else if (UserState == EUserState::Ready)
	{
		MyPS->ServerSetUserState(EUserState::Waiting);
		UpdateUserStateText(EUserState::Waiting);
	}

	// 서버 RPC
	MyPC->SendReadySignalToServer();
}

void UMyChatWidget::SendNameSettingChat(const FText& CurrentText, AMyPlayerState* MyPS, AMyPlayerController* MyPC)
{
	MyPC->Login(CurrentText);
	EditableTextBox->SetText(FText::FromString(TEXT("")));
	EditableTextBox->SetHintText(FText::FromString(TEXT("")));
	MyPC->SendMessageToServer(MyPC, FText::FromString(CurrentText.ToString() + FString(TEXT("님 입장"))));
}

void UMyChatWidget::SendNormalChat(const FText& CurrentText, AMyPlayerState* MyPS, AMyPlayerController* MyPC)
{
	const FName Username = MyPS->GetUsername();
	EditableTextBox->SetText(FText::FromString(TEXT("")));
	MyPC->SendMessageToServer(
		MyPC, FText::FromString(Username.ToString() + FString(TEXT(": ")) + CurrentText.ToString()));
}

void UMyChatWidget::SendBaseballNumber(const FText& CurrentText, AMyPlayerState* MyPS, AMyPlayerController* MyPC)
{
	FString CurrentString = CurrentText.ToString();

	auto UserState = MyPS->GetUserState();

	if (UserState == EUserState::Gaming_MyTurn)
	{
		if (CurrentString.Len() == 4 &&
			CurrentString[0] == '/' &&
			CurrentString[1] >= '1' && CurrentString[1] <= '9' &&
			CurrentString[2] >= '1' && CurrentString[2] <= '9' &&
			CurrentString[3] >= '1' && CurrentString[3] <= '9' &&
			CurrentString[2] != CurrentString[1] &&
			CurrentString[3] != CurrentString[1] &&
			CurrentString[3] != CurrentString[2]
		)
		{
			const FName Username = MyPS->GetUsername();
			EditableTextBox->SetText(FText::FromString(TEXT("")));
			MyPC->SendBaseballMessageToServer(
				MyPC, CurrentText.ToString());
		}
		else
		{
			if (ChatVerticalBox)
			{
				if (const auto ErrorMessageBlock = NewObject<UTextBlock>(ChatVerticalBox))
				{
					EditableTextBox->SetText(FText::FromString(TEXT("")));
					ErrorMessageBlock->SetText(FText::FromString(TEXT("잘못된 투구 입력")));
					ErrorMessageBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
					ChatVerticalBox->AddChild(ErrorMessageBlock);
				}
			}
		}
	}
	else
	{
		if (ChatVerticalBox)
		{
			if (const auto ErrorMessageBlock = NewObject<UTextBlock>(ChatVerticalBox))
			{
				EditableTextBox->SetText(FText::FromString(TEXT("")));
				ErrorMessageBlock->SetText(FText::FromString(TEXT("게임 중이 아니거나 본인 차례가 아님")));
				ErrorMessageBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
				ChatVerticalBox->AddChild(ErrorMessageBlock);
			}
		}
	}
}

void UMyChatWidget::UpdateUserStateText(const EUserState& UserState)
{
	switch (UserState)
	{
	case EUserState::BeforeLogin:
		UserStateTextBlock->SetText(FText::FromString(TEXT("아직 로그인 안함")));
		break;
	case EUserState::Waiting:
		UserStateTextBlock->SetText(FText::FromString(TEXT("아직 준비 안함")));
		break;
	case EUserState::Ready:
		UserStateTextBlock->SetText(FText::FromString(TEXT("준비중")));
		break;
	case EUserState::Gaming_OtherTurn:
		UserStateTextBlock->SetText(FText::FromString(TEXT("내차례아님!")));
		break;
	case EUserState::Gaming_MyTurn:
		UserStateTextBlock->SetText(FText::FromString(TEXT("내차례!")));
		break;
	case EUserState::Gaming_Out:
		UserStateTextBlock->SetText(FText::FromString(TEXT("관전중")));
		break;
	}
}

void UMyChatWidget::UpdateScoreBox(const TArray<FPlayerScoreData>& PlayerScoreDatas)
{
	ScoreVerticalBox->ClearChildren();
	for (auto [PlayerName, PlayerScore] : PlayerScoreDatas)
	{
		FString ContentString;
		ContentString.Append(PlayerName.ToString());
		ContentString.Append(TEXT(": "));
		ContentString.AppendInt(PlayerScore);
		ContentString.Append(TEXT("win"));

		UTextBlock* NewTextBlock = NewObject<UTextBlock>(ScoreVerticalBox);
		NewTextBlock->SetText(FText::FromString(ContentString));
		ScoreVerticalBox->AddChild(NewTextBlock);
	}
}

void UMyChatWidget::UpdateReadyButtonState(const bool bIsInGame)
{
	if (bIsInGame)
	{
		ReadyButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		ReadyButtonTextBlock->SetText(FText::FromString(TEXT("게임중")));
	}
	else
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		ReadyButtonTextBlock->SetText(FText::FromString(TEXT("준비버튼")));
	}
}
