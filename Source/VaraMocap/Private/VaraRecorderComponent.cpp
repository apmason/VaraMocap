// All Rights Reserved


#include "VaraRecorderComponent.h"
#include "OculusXRBodyTrackingComponent.h"
// #include "OculusMovementTypes.h"
#include "Engine/Engine.h"
#include "GeometryCollection/GeometryCollectionAlgo.h"

UVaraRecorderComponent::UVaraRecorderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVaraRecorderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRecording)
	{
		check(BodyTracker);
		if (!IsValid(BodyTracker))
		{
			return;
		}
		
		CurrentFrameNum++;
		CurrentTimestamp += DeltaTime;

		FVaraMotionCaptureFrame NewFrame;
		NewFrame.FrameNum = CurrentFrameNum;
		NewFrame.Timestamp = CurrentTimestamp;
		
		// Populate the bone transforms in the frame
		for (const auto& BonePair : BodyTracker->BoneNames)
		{
			const EOculusXRBoneID BoneID = BonePair.Key;
			const FName BoneName = BonePair.Value;
			const FTransform BoneTransform = GetTransformInParentBoneSpace(BoneName);

			// Set the corresponding member variable in the frame
			switch (BoneID)
			{
			case EOculusXRBoneID::BodyHips:
				NewFrame.Hips = BoneTransform;
				break;
			case EOculusXRBoneID::BodySpineLower:
				NewFrame.SpineLower = BoneTransform;
				break;
			case EOculusXRBoneID::BodySpineMiddle:
				NewFrame.SpineMiddle = BoneTransform;
				break;
			case EOculusXRBoneID::BodySpineUpper:
				NewFrame.SpineUpper = BoneTransform;
				break;
			case EOculusXRBoneID::BodyChest:
				NewFrame.Chest = BoneTransform;
				break;
			case EOculusXRBoneID::BodyNeck:
				NewFrame.Neck = BoneTransform;
				break;
			case EOculusXRBoneID::BodyHead:
				NewFrame.Head = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftShoulder: //// Left side
				NewFrame.LeftShoulder = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftArmUpper:
				NewFrame.LeftArmUpper = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftArmLower:
				NewFrame.LeftArmLower = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandWrist:
				NewFrame.LeftHandWrist = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandThumbMetacarpal:
				NewFrame.LeftHandThumbMetacarpal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandThumbProximal:
				NewFrame.LeftHandThumbProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandThumbDistal:
				NewFrame.LeftHandThumbDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandIndexProximal:
				NewFrame.LeftHandIndexProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandIndexIntermediate:
				NewFrame.LeftHandIndexIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandIndexDistal:
				NewFrame.LeftHandIndexDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandMiddleProximal:
				NewFrame.LeftHandMiddleProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandMiddleIntermediate:
				NewFrame.LeftHandMiddleIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandMiddleDistal:
				NewFrame.LeftHandMiddleDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandRingProximal:
				NewFrame.LeftHandRingProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandRingIntermediate:
				NewFrame.LeftHandRingIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandRingDistal:
				NewFrame.LeftHandRingDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandLittleMetacarpal:
				NewFrame.LeftHandPinkyMetacarpal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandLittleProximal:
				NewFrame.LeftHandPinkyProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandLittleIntermediate:
				NewFrame.LeftHandPinkyIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyLeftHandLittleDistal:
				NewFrame.LeftHandPinkyDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightShoulder: //// Right side
				NewFrame.RightShoulder = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightArmUpper:
				NewFrame.RightArmUpper = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightArmLower:
				NewFrame.RightArmLower = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandWrist:
				NewFrame.RightHandWrist = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandThumbMetacarpal:
				NewFrame.RightHandThumbMetacarpal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandThumbProximal:
				NewFrame.RightHandThumbProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandThumbDistal:
				NewFrame.RightHandThumbDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandIndexProximal:
				NewFrame.RightHandIndexProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandIndexIntermediate:
				NewFrame.RightHandIndexIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandIndexDistal:
				NewFrame.RightHandIndexDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandMiddleProximal:
				NewFrame.RightHandMiddleProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandMiddleIntermediate:
				NewFrame.RightHandMiddleIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandMiddleDistal:
				NewFrame.RightHandMiddleDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandRingProximal:
				NewFrame.RightHandRingProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandRingIntermediate:
				NewFrame.RightHandRingIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandRingDistal:
				NewFrame.RightHandRingDistal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandLittleMetacarpal:
				NewFrame.RightHandPinkyMetacarpal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandLittleProximal:
				NewFrame.RightHandPinkyProximal = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandLittleIntermediate:
				NewFrame.RightHandPinkyIntermediate = BoneTransform;
				break;
			case EOculusXRBoneID::BodyRightHandLittleDistal:
				NewFrame.RightHandPinkyDistal = BoneTransform;
				
			default:
				break;
			}
		}

		CurrentCapture.Frames.Add(NewFrame);
	}
}

void UVaraRecorderComponent::BeginRecording(UOculusXRBodyTrackingComponent* TrackingComponent)
{
	BodyTracker = TrackingComponent;
	bIsRecording = true;
	CurrentCapture = FVaraMotionCapture();
	CurrentCapture.CreatedAt = FDateTime::Now();
	CurrentFrameNum = 0;
	CurrentTimestamp = 0;
}

const FVaraMotionCapture& UVaraRecorderComponent::EndRecording()
{
	bIsRecording = false;
	CurrentFrameNum = 0;
	CurrentTimestamp = 0;
	BodyTracker = nullptr;
	return CurrentCapture;
}

void UVaraRecorderComponent::ClearCapture()
{
	CurrentCapture = FVaraMotionCapture();
}

FTransform UVaraRecorderComponent::GetTransformInParentBoneSpace(const FName& BoneName)
{
	FTransform BoneTransform;

	if (BoneName == FName("Hips"))
	{
		// The transform of our bone in component space
		const FTransform ComponentTrans = BodyTracker->GetBoneTransformByName(BoneName, EBoneSpaces::ComponentSpace);
		
		// Modify the rotation to transform from (90, 0, -90) to (90, 0, 0)
		FRotator NewRotation = ComponentTrans.GetRotation().Rotator();
		NewRotation.Yaw += 90;

		// The converted transform relative to the desired default transform
		BoneTransform = FTransform(NewRotation, ComponentTrans.GetTranslation(), ComponentTrans.GetScale3D());

		// This makes sure that the character's feet start on the ground
		if (CurrentFrameNum == 1)
		{
			const float TargetHeight = 94.0f;
			HipsOffset = TargetHeight - BoneTransform.GetLocation().Z;

			const FString DebugMessage = FString(TEXT("HIPS OFFSET IS %f"), HipsOffset);
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, DebugMessage);
		}
		
		FVector NewLocation = FVector(BoneTransform.GetLocation().X, BoneTransform.GetLocation().Y, BoneTransform.GetLocation().Z + HipsOffset);
		BoneTransform.SetLocation(NewLocation);
	}
	else
	{
		/** Get the parent name and the parent transform */
		const FName ParentName = BodyTracker->GetParentBone(BoneName);
		const FTransform ParentTrans = BodyTracker->GetBoneTransformByName(ParentName, EBoneSpaces::ComponentSpace);

		/** The transform of our bone in component space */
		const FTransform ComponentTrans = BodyTracker->GetBoneTransformByName(BoneName, EBoneSpaces::ComponentSpace);

		/** The converted transform in parent bone space */
		BoneTransform = ComponentTrans.GetRelativeTransform(ParentTrans);
	}

	return BoneTransform;
}
