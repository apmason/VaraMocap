// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VaraMotionCapture.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FVaraMotionCaptureFrame
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	int32 FrameNum;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	float Timestamp;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform Root;
	
    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform Hips;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform SpineLower;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform SpineMiddle;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform SpineUpper;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform Chest;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform Neck;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform Head;

	// Left arm

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftShoulder;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftArmUpper;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftArmUpperMid;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftArmUpperDistal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftArmLower;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftArmLowerMid;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftArmLowerDistal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandWrist;

	// Left Thumb

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftHandThumbTrapezium;
	
    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandThumbMetacarpal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandThumbProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandThumbDistal;

	// Left index
	
	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftHandIndexProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandIndexIntermediate;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandIndexDistal;

    // Left middle

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftHandMiddleProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandMiddleIntermediate;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandMiddleDistal;

    // Left ring

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform LeftHandRingProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandRingIntermediate;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandRingDistal;

    // Left Pinky

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandPinkyMetacarpal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandPinkyProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandPinkyIntermediate;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform LeftHandPinkyDistal;

	// Right arm

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightShoulder;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightArmUpper;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightArmLower;

    // Right hand

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandWrist;

	// thumb
	
    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandThumbMetacarpal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandThumbProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandThumbDistal;

	// index

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandIndexProximal;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandIndexIntermediate;

    UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
    FTransform RightHandIndexDistal;

	// middle

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandMiddleProximal;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandMiddleIntermediate;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandMiddleDistal;

	// ring
	
	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandRingProximal;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandRingIntermediate;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandRingDistal;

	// pinky

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandPinkyMetacarpal;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandPinkyProximal;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandPinkyIntermediate;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FTransform RightHandPinkyDistal;
	
};

USTRUCT(BlueprintType)
struct FVaraMotionCapture
{
	GENERATED_USTRUCT_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	int32 Id;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	int32 CreatorId;
	
	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	TArray<FVaraMotionCaptureFrame> Frames;

	UPROPERTY(BlueprintReadWrite, Category = "Motion Capture")
	FDateTime CreatedAt;
	
};
