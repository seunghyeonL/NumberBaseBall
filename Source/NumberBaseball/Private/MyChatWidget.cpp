// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChatWidget.h"

#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"

void UMyChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox)
	{
		EditableTextBox->SetHintText(FText::FromString(TEXT("사용할 닉네임 입력")));
		EditableTextBox->OnTextCommitted.AddDynamic(this, &UMyChatWidget::OnTextCommitted);
	}
}

void UMyChatWidget::OnTextCommitted(const FText& CurrentText, ETextCommit::Type CommitType)
{
	if (CommitType != ETextCommit::OnEnter) return;

	const auto MyPC = GetOwningPlayer<AMyPlayerController>();
	if (!MyPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Owning PlayerController."));
		return;
	}

	const auto MyPS = MyPC->GetPlayerState<AMyPlayerState>();
	if (!MyPS)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Matching PlayerState."));
		return;
	}

	if (!MyPS->GetIsNameSet())
	{
		MyPS->Login(CurrentText);
		EditableTextBox->SetText(FText::FromString(TEXT("")));
		EditableTextBox->SetHintText(FText::FromString(TEXT("")));
		MyPC->SendMessageToServer(FText::FromString(CurrentText.ToString() + FString(TEXT("님 입장"))));
	}
	else
	{
		const FName Username = MyPS->GetUsername();
		EditableTextBox->SetText(FText::FromString(TEXT("")));
		MyPC->SendMessageToServer(FText::FromString(Username.ToString() + FString(TEXT(": ")) + CurrentText.ToString()));
	}
}
