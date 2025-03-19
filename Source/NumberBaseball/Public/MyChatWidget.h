// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types.h"
#include "MyChatWidget.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;
class UScrollBox;
class UEditableTextBox;
class UCanvasPanel;

class AMyPlayerController;
class AMyPlayerState;

UCLASS()
class NUMBERBASEBALL_API UMyChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Input TextBox
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox;

	// Chat Box
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ChatVerticalBox;

	// History Box
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> HistoryVerticalBox;

	// Score Box
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ScoreVerticalBox;

	// Ready Button
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReadyButtonTextBlock;

	// Turn Display TextBlock
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UserStateTextBlock;

	// Commit Handler
	UFUNCTION()
	void OnTextCommitted(const FText& CurrentText, ETextCommit::Type CommitType);

	UFUNCTION()
	void OnReadyButtonClicked();
	
	void UpdateUserStateText(const EUserState& UserState);
	void UpdateScoreBox();
	void UpdateReadyButtonState(const bool bIsInGame);

	virtual void NativeConstruct() override;

private:
	void SendNameSettingChat(const FText& CurrentText, AMyPlayerState* MyPS, AMyPlayerController* MyPC);
	void SendNormalChat(const FText& CurrentText, AMyPlayerState* MyPS, AMyPlayerController* MyPC);
	void SendBaseballNumber(const FText& CurrentText, AMyPlayerState* MyPS, AMyPlayerController* MyPC);
};

