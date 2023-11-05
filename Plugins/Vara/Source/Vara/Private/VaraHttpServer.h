// All Rights Reserved

#pragma once

#include "CoreMinimal.h"

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

private:

	void ParseJsonString(const FString& JsonString);
	
	FTransform GetTransformFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	FVector GetVectorFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	FQuat GetQuatFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
};
