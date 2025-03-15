// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyFuncLib.generated.h"

UCLASS()
class NUMBERBASEBALL_API UMyFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static FString GetBallResult(FString ServerBall, FString PlayerBall);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static FString GenerateRandomNumberString();
};
