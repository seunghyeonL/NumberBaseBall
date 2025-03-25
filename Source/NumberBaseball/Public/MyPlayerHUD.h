// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerHUD.generated.h"

UCLASS()
class NUMBERBASEBALL_API AMyPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyPlayerHUD();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> ChatWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowChatWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UUserWidget> ChatWidgetInstance;
};
