// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VaraMotionCapture.h"
#include "VaraMocapFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UVaraMocapFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MoCap Function Library")
	static void BodyCaptureToJsonString(const FVaraMotionCapture& InCapture, FString& OutJsonString);
	
};
