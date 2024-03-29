// Copyright Jason 2023. All rights Reserved.

#include "CallWXYY.h"

UCallWXYY *UCallWXYY::CallWXYY(UObject *WorldContextObject, const FWXSettings &settings)
{
    // new obj
    UCallWXYY *CallWXYY = NewObject<UCallWXYY>();
    // set obj
    CallWXYY->settings = settings;
    CallWXYY->WorldContextObject = WorldContextObject;
    CallWXYY->RegisterWithGameInstance(WorldContextObject);
    return CallWXYY;
}

void UCallWXYY::Activate()
{
    // Tonke is empty
    if (settings.Token.IsEmpty())
    {
        Recieved.Broadcast({}, TEXT("Token is not set"), EOAResponseStatus::FAILED);
        UE_LOG(LogTemp, Error, TEXT("Token is not set"));
    }

    // new Http request
    auto HttpRequest = FHttpModule::Get().CreateRequest();
    FString url;
    switch (settings.Model)
    {
    case EModels::ERNIE_Bot:
        url = TEXT("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=" + settings.Token);
        break;

    case EModels::ERNIE_Bot_turbo:
        url = TEXT("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/eb-instant?access_token=" + settings.Token);
        break;
    }
    HttpRequest->SetURL(url);
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> HttpBody = MakeShareable(new FJsonObject());

    if (!settings.messages.Num() == 0)
    {
        TArray<TSharedPtr<FJsonValue>> MessagesArray;
        FString role;
        for (int i = 0; i < settings.messages.Num(); i++)
        {
            switch (settings.messages[i].role)
            {
            case EOAChatRole::USER:
                role = TEXT("user");
                break;
            case EOAChatRole::ASSISTANT:
                role = TEXT("assistant");
                break;
            }
            TSharedPtr<FJsonObject> message = MakeShared<FJsonObject>();
            message->SetStringField(TEXT("role"), role);
            message->SetStringField(TEXT("content"), settings.messages[i].content);
            MessagesArray.Add(MakeShareable(new FJsonValueObject(message)));
        }
        HttpBody->SetArrayField(TEXT("messages"), MessagesArray);
    }
    HttpBody->SetBoolField(TEXT("stream"), settings.Stream);

    FString Bodystr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Bodystr);
    FJsonSerializer::Serialize(HttpBody.ToSharedRef(), Writer);

    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetContentAsString(Bodystr);

    if (HttpRequest->ProcessRequest())
    {
        if (settings.Stream)
        {
            // TODO 流式响应
        }

        HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCallWXYY::OnResponse);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Bodystr:%s"), *Bodystr);
        Recieved.Broadcast({}, TEXT("Error sending request"), EOAResponseStatus::FAILED);
    }
}

void UCallWXYY::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!WasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("Error processing request. \n%s \n%s"), *Response->GetContentAsString(), *Response->GetURL());
        Recieved.Broadcast({}, *Response->GetContentAsString(), EOAResponseStatus::FAILED);
    }
    TSharedPtr<FJsonObject> responseObject;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    if (FJsonSerializer::Deserialize(reader, responseObject))
    {
        bool err = responseObject->HasField(TEXT("error_code"));

        if (err)
        {
            UE_LOG(LogTemp, Error, TEXT("%s"), *Response->GetContentAsString());
            Recieved.Broadcast({}, TEXT("Api error"), EOAResponseStatus::FAILED);
            return;
        }
        FChatLog _out;
        _out.role = EOAChatRole::ASSISTANT;
        _out.content = responseObject->GetStringField(TEXT("result"));
        Recieved.Broadcast(_out, "", EOAResponseStatus::FINISHED);
    }
}