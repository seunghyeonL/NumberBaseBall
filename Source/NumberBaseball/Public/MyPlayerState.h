// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NUMBERBASEBALL_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User|Username")
	FName Username;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User|State")
	bool bIsNameSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User|State")
	EUserState UserState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User|State")
	bool bIsOuted;

public:
	AMyPlayerState();

	void Login(const FText& NewUsername);

	bool GetIsNameSet() const { return bIsNameSet; }
	FName GetUsername() const { return Username; }
	void SetUsername(const FName& NewUsername) { Username = NewUsername; }
};
