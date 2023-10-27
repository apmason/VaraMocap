// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VaraCountdownComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVaraRecordingEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVaraCountdownEvent, int32, CountdownRemaining);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UVaraCountdownComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVaraCountdownComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Vara Countdown")
	FVaraRecordingEvent OnRecordingBegan;

	UPROPERTY(BlueprintAssignable, Category = "Vara Countdown")
	FVaraRecordingEvent OnRecordingEnded;

	UPROPERTY(BlueprintAssignable, Category = "Vara Countdown")
	FVaraCountdownEvent OnCountdown;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Vara Countdown")
	void BeginCountdown();

	UFUNCTION(BlueprintCallable, Category = "Vara Countdown")
	void EndCountdown();

	/**
	 * @brief 
	 * @return The required time that component needs to be held down for before starting.
	 */
	UFUNCTION(BlueprintPure, Category = "Vara Countdown")
	float GetCountdownTime() const { return CountdownTime; }

	UFUNCTION(BlueprintPure, Category = "Vara Countdown")
	bool GetIsRecording() const { return bIsRecording; }
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void BeginRecording();

	void EndRecording();
	
private:
	/**
	 * @brief Time in seconds that need to elapse before a recording will begin.
	 */
	float CountdownTime = 3.0f;

	/**
	 * @brief Is a count down in progress. Before starting or ending a recording.
	 */
	bool bCountingDown = false;

	/**
	 * @brief How long has elapsed in the countdown.
	 */
	float ElapsedCountdownTime = 0.0f;

	/**
	 * @brief Are we currently recording.
	 */
	bool bIsRecording = false;

	/**
	 * @brief Used to know what that last CountdownRemaining time is.
	 */
	int32 LastSecondCalled = -1;
};
