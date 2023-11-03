#include "VaraAsyncCallRoute.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogCallRoute);

UVaraAsyncCallRoute* UVaraAsyncCallRoute::CallPostRoute(const FString& InURL, const FString& InJsonString)
{
	UVaraAsyncCallRoute* AsyncNode = NewObject<UVaraAsyncCallRoute>();
	AsyncNode->bIsGetRoute = false;
	AsyncNode->URL = InURL;
	AsyncNode->JsonString = InJsonString;
	return AsyncNode;
}

UVaraAsyncCallRoute* UVaraAsyncCallRoute::CallGetRoute(const FString& InURL)
{
	UVaraAsyncCallRoute* AsyncNode = NewObject<UVaraAsyncCallRoute>();
	AsyncNode->bIsGetRoute = true;
	AsyncNode->URL = InURL;
	return AsyncNode;
}

void UVaraAsyncCallRoute::Activate()
{
	Super::Activate();

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UVaraAsyncCallRoute::OnNetworkResponseReceived);
	Request->SetURL(URL);
	
	if (bIsGetRoute)
	{
		Request->SetVerb("GET");
	} 
	else
	{
		Request->SetVerb("POST");

		Request->SetHeader("Content-Type", "application/json");
		Request->SetContentAsString(JsonString);
	}

	Request->SetTimeout(30.0f);
	Request->ProcessRequest();
}

void UVaraAsyncCallRoute::OnNetworkResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	Request->OnProcessRequestComplete().Unbind();
	UE_LOG(LogCallRoute, Log, TEXT("Retrieved network response"));

	if (bConnectedSuccessfully)
	{
		UE_LOG(LogCallRoute, Log, TEXT("The response connected successfully"));

		if (Response->GetResponseCode() == 200)
		{
			UE_LOG(LogCallRoute, Log, TEXT("The response code is 200"));
			HandleSuccess(Response->GetContentAsString());
		}
		else
		{
			UE_LOG(LogCallRoute, Log, TEXT("Response code is %d"), Response->GetResponseCode());
			
			TSharedPtr<FJsonObject> ResponseObj;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			FJsonSerializer::Deserialize(Reader, ResponseObj);

			if (ResponseObj->HasField("message"))
			{
				FString Message = ResponseObj->GetStringField("message");
				UE_LOG(LogCallRoute, Log, TEXT("Successful response with a message of %s"), *Message);
			}
			
			UE_LOG(LogCallRoute, Log, TEXT("The response code is not 200"));
			HandleFailure(Response->GetContentAsString());
		}
	}
	else
	{
		UE_LOG(LogCallRoute, Log, TEXT("Request failed because HTTP request couldn't connect"));
		HandleFailure(Response->GetContentAsString());
	}
}

void UVaraAsyncCallRoute::HandleSuccess(const FString& ResponseContentString)
{
	AsyncTask(ENamedThreads::GameThread, [this, ResponseContentString]()
	{
		OnSuccess.Broadcast(ResponseContentString);
		SetReadyToDestroy();
	});
}

void UVaraAsyncCallRoute::HandleFailure(const FString& ResponseContentString)
{
	AsyncTask(ENamedThreads::GameThread, [this, ResponseContentString]()
	{
		OnFailure.Broadcast(ResponseContentString);
		SetReadyToDestroy();
	});
}