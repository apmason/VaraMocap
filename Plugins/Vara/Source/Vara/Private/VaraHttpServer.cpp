// All Rights Reserved


#include "VaraHttpServer.h"
#include "HttpServerModule.h"
#include "HttpPath.h"
#include "IHttpRouter.h"

FVaraHttpServer::FVaraHttpServer()
{
}

FVaraHttpServer::~FVaraHttpServer()
{
}

void FVaraHttpServer::Start()
{
	TSharedPtr<IHttpRouter> HttpRouter = FHttpServerModule::Get().GetHttpRouter(8080);
	
	FHttpPath Route = FHttpPath(TEXT("/api/hello-world"));

	const FHttpRequestHandler OnCallback = [&](const FHttpServerRequest& ServerRequest, const FHttpResultCallback& ResultCallback) -> bool
	{
		// Get the JSON as an FString
		FString RequestBody;
		if (ServerRequest.Body.Num() > 0)
		{
			RequestBody = FString(UTF8_TO_TCHAR(ServerRequest.Body.GetData()));

			// parse
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Route is being handled!"));
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("{\"success\": true}"), TEXT("application/json"));
		ResultCallback(MoveTemp(Response));
		return true;
	};
	HttpRouter->BindRoute(Route, EHttpServerRequestVerbs::VERB_POST, OnCallback);

	FHttpServerModule::Get().StartAllListeners();
}

void FVaraHttpServer::Stop()
{
	
}
