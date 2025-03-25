// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	FName Username;

	UPROPERTY(Replicated)
	EUserState UserState;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	AMyPlayerState();
	
	EUserState GetUserState() const { return UserState; }
	FName GetUsername() const { return Username; }

	UFUNCTION(Server, Reliable)
	void ServerSetUsername(const FName& NewUsername);
	
	UFUNCTION(Server, Reliable)
	void ServerSetUserState(const EUserState& NewUserState);
};






