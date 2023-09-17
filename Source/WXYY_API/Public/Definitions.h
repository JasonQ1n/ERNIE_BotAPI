// Copyright Jason 2023. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Http.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Json.h"
#include "JsonObjectConverter.h"
#include "JsonObjectWrapper.h"
#include "Internationalization/Text.h"
#include "Definitions.generated.h"

UENUM(BlueprintType)
enum class EOAChatRole : uint8
{
	USER = 0,
	ASSISTANT = 1,
};

USTRUCT(BlueprintType)
struct FChatLog
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	EOAChatRole role = EOAChatRole::USER;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	FString content = "";
};

/**
 * 结构体 appid 和 appsecret
 */
USTRUCT(BlueprintType)
struct FAppInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	FString AppId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	FString AppSecret;
};

/**
 * 枚举值 EModels
 */
UENUM(BlueprintType)
enum class EModels : uint8
{
	// ERNIE-Bot是百度自行研发的大语言模型，覆盖海量中文数据，具有更强的对话问答、内容创作生成等能力。本文介绍了千帆相关API，以及如何使用API与文心一言进行对话交互。
	ERNIE_Bot = 0,
	// ERNIE-Bot-turbo是百度自行研发的大语言模型，覆盖海量中文数据，具有更强的对话问答、内容创作生成等能力，响应速度更快。本文介绍了千帆相关API，以及如何使用API与文心一言进行对话交互。内置ERNIE-Bot-turbo系列最新版模型ERNIE-Bot-turbo-0725。
	ERNIE_Bot_turbo = 1,
};

/**
 * 结构体 Setting
 * @param Token 百度AI开放平台的Token
 * @param Model 模型
 * @param Stream 是否流式
 * @param messages 消息
 * @param delimiters 分隔符
 */
USTRUCT(BlueprintType)
struct FWXSettings
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	FString Token;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	EModels Model = EModels::ERNIE_Bot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	bool Stream = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WXYY_API")
	TArray<FChatLog> messages;
};

/**
 * 枚举值 EOAResponseStatus
 */
UENUM(BlueprintType)
enum class EOAResponseStatus : uint8
{
	FAILED = 0,
	UPDATE = 1,
	FINISHED = 2,
};