// All Rights Reserved

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FVaraConfig
{
public:
	static const FString& GetIpAddr()
	{
		return FString(TEXT("http://192.168.0.22:8080"));
	}
};
