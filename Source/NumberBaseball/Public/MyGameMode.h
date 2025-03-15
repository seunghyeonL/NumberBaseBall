// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Server")
	TArray<FUniqueNetIdRepl> OrderOfPlayers;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Server")
	int32 CurrentTurnIdx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Server")
	FString ServerBall;

	UFUNCTION()
	void GameStart();
};
