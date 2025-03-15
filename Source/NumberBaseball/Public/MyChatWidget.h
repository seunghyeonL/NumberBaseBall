// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyChatWidget.generated.h"

class UEditableTextBox;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API UMyChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox;

	UFUNCTION()
	void OnTextCommitted(const FText& NewText, ETextCommit::Type CommitType);

	virtual void NativeConstruct() override;
};
