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
