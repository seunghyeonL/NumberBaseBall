// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API AMyGameState : public AGameState
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameState")
	bool bIsInGame;

public:
	bool GetIsInGame() const { return bIsInGame; }
	
	UFUNCTION(NetMulticast, Reliable)
	void BroadCastMessage(const FText& Message);
};
