// Copyright Jason 2023. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "HttpModule.h"
#include "Definitions.h"
#include "CallWXYY.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWXYYResponseRecieved, const FChatLog &, Message, const FString &, errorMessage, EOAResponseStatus, Status);
UCLASS()
class WXYY_API_API UCallWXYY : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	FWXSettings settings;
	UPROPERTY(BlueprintAssignable, Category = "WXYY_API")
	FOnWXYYResponseRecieved Recieved;

private:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "WXYY_API")
	static UCallWXYY *CallWXYY(UObject *WorldContextObject, const FWXSettings &settings);

	virtual void Activate() override;

	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
	const UObject *WorldContextObject;
};
