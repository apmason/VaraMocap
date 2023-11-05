// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VaraMotionCapture.h"

DECLARE_DELEGATE_OneParam(FVaraServerEvent, FVaraMotionCapture Capture);

/**
 * 
 */
class FVaraHttpServer
{
public:
	FVaraHttpServer();
	~FVaraHttpServer();

	void Start();
	void Stop();

	FVaraServerEvent OnServerEvent;

private:

	void ParseJsonString(const FString& JsonString);
	
	FTransform GetTransformFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	FVector GetVectorFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	FQuat GetQuatFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
};
