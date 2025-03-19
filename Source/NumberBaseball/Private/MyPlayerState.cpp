// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

#include "MyChatWidget.h"
#include "MyPlayerController.h"
#include "MyPlayerHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, Username);
	DOREPLIFETIME(AMyPlayerState, UserState);
} 

AMyPlayerState::AMyPlayerState()
{
	UserState = EUserState::BeforeLogin;
	Username = TEXT("Anonymous");
}

void AMyPlayerState::ServerSetUsername_Implementation(const FName& NewUsername)
{
	Username = NewUsername;
}

void AMyPlayerState::ServerSetUserState_Implementation(const EUserState& NewUserState)
{
	UserState = NewUserState;
}

// UE_DISABLE_OPTIMIZATION
// void AMyPlayerState::OnRep_UserState()
// {
// 	AController* Test = GetOwningController();
// 	auto MyPC = Cast<AMyPlayerController>(Test);
// 	// FString LocalRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole());
// 	// FString RemoteRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole());
// 	// FString NetModeString = GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server");
// 	// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s %s %s"), *NetModeString, *LocalRoleString, *RemoteRoleString));
// 	
// 	if (!MyPC)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("OnRep_UserState: PlayerController is null"));
// 		return;
// 	}
//
// 	auto MyPlayerHUD = Cast<AMyPlayerHUD>(MyPC->MyHUD);
// 	if (!MyPlayerHUD)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("OnRep_UserState: MyHUD is null"));
// 		return;
// 	}
//
// 	auto ChatWidget = Cast<UMyChatWidget>(MyPlayerHUD->ChatWidgetInstance);
// 	if (!ChatWidget)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("OnRep_UserState: ChatWidget is null"));
// 		return;
// 	}
//
// 	ChatWidget->UpdateUserStateText(UserState);
// }
// UE_ENABLE_OPTIMIZATION