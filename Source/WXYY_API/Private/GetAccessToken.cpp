// Copyright Jason 2023. All rights Reserved.

#include "GetAccessToken.h"
#include "Http.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UGetAccessToken *UGetAccessToken::GetAccessToken(UObject *WorldContextObject, const FAppInfo &AppInfo)
{
    UGetAccessToken *GetAccessToken = NewObject<UGetAccessToken>();
    GetAccessToken->AppInfo = AppInfo;
    GetAccessToken->WorldContextObject = WorldContextObject;
    GetAccessToken->RegisterWithGameInstance(WorldContextObject);
    return GetAccessToken;
}

void UGetAccessToken::Activate()
{
    // AppId or AppSecret is empty
    if (AppInfo.AppId.IsEmpty() || AppInfo.AppSecret.IsEmpty())
    {
        OnResponseRecieved.Broadcast(TEXT(""), TEXT("AppId or AppSecret is empty"), false);
        return;
    }

    // new Http request
    auto HttpRequest = FHttpModule::Get().CreateRequest();
    FString url = TEXT("https://aip.baidubce.com/oauth/2.0/token?client_id=" + AppInfo.AppId + "&client_secret=" + AppInfo.AppSecret) + "&grant_type=client_credentials";
    HttpRequest->SetURL(url);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    if (!HttpRequest->ProcessRequest())
    {
        OnResponseRecieved.Broadcast(TEXT(""), TEXT("Request failed"), false);
        return;
    }
    // bind callback
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UGetAccessToken::OnRequest);
}

void UGetAccessToken::OnRequest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // request failed
    if (!bWasSuccessful)
    {
        OnResponseRecieved.Broadcast(TEXT(""), *Response->GetContentAsString(), false);
        return;
    }

    // parse json
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        // get access_token
        FString AccessToken = JsonObject->GetStringField(TEXT("access_token"));
        OnResponseRecieved.Broadcast(AccessToken, TEXT(""), true);
    }
    else
    {
        OnResponseRecieved.Broadcast(TEXT(""), TEXT("Parse json failed"), false);
    }
}