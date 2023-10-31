// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VaraMotionCapture.h"
#include "VaraRecorderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UVaraRecorderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVaraRecorderComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void BeginRecording(UOculusXRBodyTrackingComponent* TrackingComponent);
	
	UFUNCTION(BlueprintCallable)
	const FVaraMotionCapture& EndRecording();

	UFUNCTION(BlueprintCallable)
	void ClearCapture();
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsRecording;
	
private:

	/** Given a BoneName, get the transform in parent bone space */
	FTransform GetTransformInParentBoneSpace(const FName& BoneName);
	
	UPROPERTY()
	class UOculusXRBodyTrackingComponent* BodyTracker;

	UPROPERTY()
	FVaraMotionCapture CurrentCapture;
	
	UPROPERTY()
	int32 CurrentFrameNum;

	UPROPERTY()
	float CurrentTimestamp;

	UPROPERTY()
	float HipsOffset;
		
};
