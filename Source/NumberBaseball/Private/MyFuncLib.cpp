// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberBaseball/Public/MyFuncLib.h"

FString UMyFuncLib::GetBallResult(FString ServerBall, FString PlayerBall)
{
	FString Result = TEXT("");
	TMap<char, int32> ResultMap;
	
	for (int32 i = 0 ; i < 3 ; i++)
	{
		for (int32 j = 0 ; j < 3 ; j++)
		{
			if (PlayerBall[i] == ServerBall[j])
			{
				if (i == j) ResultMap.FindOrAdd('S')++;
				else ResultMap.FindOrAdd('B')++;
			}
		}
	}
	
	Result.AppendInt(ResultMap.FindOrAdd('S'));
	Result.AppendChar('S');
	Result.AppendInt(ResultMap.FindOrAdd('B'));
	Result.AppendChar('B');
	return Result;
}

FString UMyFuncLib::GenerateRandomNumberString()
{
	FString Result = TEXT("");
	
	int32 First = FMath::RandRange(1, 9);
	
	int32 Second = FMath::RandRange(1, 9);
	while (Second == First)
	{
		Second = FMath::RandRange(1, 9);
	}
	
	int32 Third = FMath::RandRange(1, 9);
	while (Third == First || Third == Second)
	{
		Third = FMath::RandRange(1, 9);
	}

	Result.AppendInt(First);
	Result.AppendInt(Second);
	Result.AppendInt(Third);
	return Result;
}
