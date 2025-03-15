#pragma once

#include "CoreMinimal.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EUserState: uint8
{
	BeforeLogin, Waiting, Ready, Gaming
};