// Fill out your copyright notice in the Description page of Project Settings.


#include "VaraCountdownComponent.h"

UVaraCountdownComponent::UVaraCountdownComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UVaraCountdownComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UVaraCountdownComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCountingDown)
	{
		ElapsedCountdownTime += DeltaTime;

		// Calculate the remaining time in seconds
		const int32 CountdownRemaining = FMath::CeilToInt(CountdownTime - ElapsedCountdownTime);

		// Check if we've entered a new second
		const int32 CurrentSecond = FMath::FloorToInt(ElapsedCountdownTime);

		if (LastSecondCalled != CurrentSecond)
		{
			LastSecondCalled = CurrentSecond;

			// Call the delegate with the remaining countdown time
			OnCountdown.Broadcast(CountdownRemaining);
		}

		// Start recording
		if (ElapsedCountdownTime >= CountdownTime)
		{
			bCountingDown = false;
			ElapsedCountdownTime = 0.0f;
			LastSecondCalled = -1;

			if (bIsRecording)
			{
				EndRecording();
			}
			else
			{
				BeginRecording();
			}
		}
	}
}

void UVaraCountdownComponent::BeginRecording()
{
	bIsRecording = true;
	OnRecordingBegan.Broadcast();
}

void UVaraCountdownComponent::EndRecording()
{
	bIsRecording = false;
	OnRecordingEnded.Broadcast();
}

void UVaraCountdownComponent::BeginCountdown()
{
	bCountingDown = true;
	ElapsedCountdownTime = 0.0f;
}

void UVaraCountdownComponent::EndCountdown()
{
	
}
