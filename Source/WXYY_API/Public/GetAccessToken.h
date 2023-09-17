// Copyright Jason 2023. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "HttpModule.h"
#include "Definitions.h"
#include "GetAccessToken.generated.h"

/**
 * 三个参数的动态多播代理
 * @param AccessToken 返回的AccessToken
 * @param ErrorMsg 错误信息
 * @param IsSuccess 是否成功
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResponseRecieved, const FString &, AccessToken, const FString &, ErrorMsg, bool, Success);

/**
 * 异步 HTTP 请求，获取AccessToken
 */
UCLASS()
class WXYY_API_API UGetAccessToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * AppInfo 结构体 AppId 和 AppSecret
	 */
	FAppInfo AppInfo;
	/**
	 * 三个参数的动态多播代理
	 * @param AccessToken 返回的AccessToken
	 * @param ErrorMsg 错误信息
	 * @param IsSuccess 是否成功
	 */
	UPROPERTY(BlueprintAssignable, Category = "WXYY_API")
	FOnResponseRecieved OnResponseRecieved;

private:
	/**
	 * 异步 HTTP 请求，获取AccessToken
	 * @param AppInfo AppInfo 结构体 AppId 和 AppSecret
	 * @return UGetAccessToken Instance
	 */
	UFUNCTION(BlueprintCallable, Category = "WXYY_API", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UGetAccessToken *GetAccessToken(UObject *WorldContextObject, const FAppInfo &AppInfo);
	/**
	 * 重载 Activate 方法
	 */
	virtual void Activate() override;
	/**
	 * 请求成功回调
	 */
	void OnRequest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	/**
	 * 声明世界上下文对象
	 */
	const UObject *WorldContextObject;
};
