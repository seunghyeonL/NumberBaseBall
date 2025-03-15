// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void SendMessageToServer(const FText& Message);
};
