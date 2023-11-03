#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "VaraAsyncCallRoute.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCallRoute, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCallRouteResult, FString, ResponseContentString);

/**
 * 
 */
UCLASS()
class UVaraAsyncCallRoute : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/** 
	 * Assumes that the HTTP response will contain a boolean field named 'success' and will determine
	 * the success state based on that.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Flow Control")
	static UVaraAsyncCallRoute* CallPostRoute(const FString& InURL, const FString& InJsonString);

	/** Call an HTTP GET route using the InURL parameter. All URL parameters should already be added e.g, http://test.com/render?token=123 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Flow Control")
	static UVaraAsyncCallRoute* CallGetRoute(const FString& InURL);

	virtual void Activate() override;

public:

	UPROPERTY(BlueprintAssignable)
	FCallRouteResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FCallRouteResult OnFailure;

protected:

	void OnNetworkResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

private:

	void HandleSuccess(const FString& ResponseContentString);

	void HandleFailure(const FString& ResponseContentString);

public:
	/**
	* The serialized JSON string to pass along in our request
	*/
	UPROPERTY()
	FString JsonString;

	UPROPERTY()
	FString URL;

	UPROPERTY()
	bool bIsGetRoute = false;

};
