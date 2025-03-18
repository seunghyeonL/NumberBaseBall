#pragma once

#include "CoreMinimal.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EUserState: uint8
{
	BeforeLogin, Waiting, Ready, Gaming_MyTurn, Gaming_OtherTurn, Gaming_Out
};

USTRUCT(BlueprintType)
struct FPlayerScoreData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerScore;
};