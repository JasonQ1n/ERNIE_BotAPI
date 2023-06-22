// Copyright Jason 2023. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Definitions.generated.h"

UENUM(BlueprintType)
enum class EOAChatRole:uint8
{
    USER=0,
    ASSISTANT=1,
};

USTRUCT(BlueprintType)
struct FAnswer
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	EOAChatRole role = EOAChatRole::USER;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	FString content = "";
};