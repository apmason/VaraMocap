// All Rights Reserved


#include "VaraHttpServer.h"
#include "HttpServerModule.h"
#include "HttpPath.h"
#include "IHttpRouter.h"
#include "Dom/JsonObject.h"
#include "VaraMotionCapture.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FVaraHttpServer::FVaraHttpServer()
{
}

FVaraHttpServer::~FVaraHttpServer()
{
}

void FVaraHttpServer::Start()
{
	TSharedPtr<IHttpRouter> HttpRouter = FHttpServerModule::Get().GetHttpRouter(8080);
	
	FHttpPath Route = FHttpPath(TEXT("/api/hello-world"));

	const FHttpRequestHandler OnCallback = [&](const FHttpServerRequest& ServerRequest, const FHttpResultCallback& ResultCallback) -> bool
	{
		// Get the JSON as an FString
		FString RequestBody;
		if (ServerRequest.Body.Num() > 0)
		{
			RequestBody = FString(UTF8_TO_TCHAR(ServerRequest.Body.GetData()));
			
			ParseJsonString(RequestBody);
		}
		
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("{\"success\": true}"), TEXT("application/json"));
		ResultCallback(MoveTemp(Response));
		return true;
	};
	HttpRouter->BindRoute(Route, EHttpServerRequestVerbs::VERB_POST, OnCallback);

	FHttpServerModule::Get().StartAllListeners();
}

void FVaraHttpServer::Stop()
{
	
}

void FVaraHttpServer::ParseJsonString(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		const TSharedPtr<FJsonObject>* CaptureObjectPtr;
		if(JsonObject->TryGetObjectField(TEXT("capture"), CaptureObjectPtr))
		{
			TSharedPtr<FJsonObject> CaptureObject = *CaptureObjectPtr;

			// Not using id or createdAt yet, may in the future
			/*int32 Id;
			check(JsonObject->TryGetNumberField("id", Id));
			
			FString CreatedAtString;
			check(JsonObject->TryGetStringField("createdAt", CreatedAtString));
			*/
			
			const TArray<TSharedPtr<FJsonValue>>* JsonFrames = nullptr;
			if (CaptureObject->TryGetArrayField("frames", JsonFrames))
			{
				FVaraMotionCapture Capture = FVaraMotionCapture();
				// Capture.Id = Id;

				/*FDateTime CreatedAtDate;
				if (FDateTime::ParseIso8601(*CreatedAtString, CreatedAtDate))
				{
					Capture.CreatedAt = CreatedAtDate;
				}
				else
				{
					checkNoEntry();
				}*/

				TArray<FVaraMotionCaptureFrame> Frames;
				for (const auto& JsonValFrame : *JsonFrames)
				{
					TSharedPtr<FJsonObject> JsonObjFrame = JsonValFrame->AsObject();
					check(JsonObjFrame.IsValid());

					FVaraMotionCaptureFrame MotionCaptureFrame;
					MotionCaptureFrame.FrameNum = JsonObjFrame->GetIntegerField("frameNum");
					MotionCaptureFrame.Timestamp = JsonObjFrame->GetNumberField("timestamp");

					MotionCaptureFrame.Root = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("root"));
					MotionCaptureFrame.Hips = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("hips"));
					MotionCaptureFrame.SpineLower = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("spineLower"));
					MotionCaptureFrame.SpineMiddle = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("spineMiddle"));
					MotionCaptureFrame.SpineUpper = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("spineUpper"));
					MotionCaptureFrame.Chest = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("chest"));
					MotionCaptureFrame.Neck = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("neck"));
					MotionCaptureFrame.Head = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("head"));

					MotionCaptureFrame.LeftShoulder = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftShoulder"));
				    MotionCaptureFrame.LeftArmUpper = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftArmUpper"));
				    MotionCaptureFrame.LeftArmLower = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftArmLower"));
				    MotionCaptureFrame.LeftHandWrist = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandWrist"));

					MotionCaptureFrame.RightShoulder = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightShoulder"));
					MotionCaptureFrame.RightArmUpper = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightArmUpper"));
					MotionCaptureFrame.RightArmLower = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightArmLower"));
					MotionCaptureFrame.RightHandWrist = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandWrist"));

					MotionCaptureFrame.LeftHandThumbMetacarpal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandThumbMetacarpal"));
					MotionCaptureFrame.LeftHandThumbProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandThumbProximal"));
					MotionCaptureFrame.LeftHandThumbDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandThumbDistal"));

					MotionCaptureFrame.RightHandThumbMetacarpal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandThumbMetacarpal"));
					MotionCaptureFrame.RightHandThumbProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandThumbProximal"));
					MotionCaptureFrame.RightHandThumbDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandThumbDistal"));

					MotionCaptureFrame.LeftHandIndexProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandIndexProximal"));
					MotionCaptureFrame.LeftHandIndexIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandIndexIntermediate"));
					MotionCaptureFrame.LeftHandIndexDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandIndexDistal"));
					
					MotionCaptureFrame.RightHandIndexProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandIndexProximal"));
					MotionCaptureFrame.RightHandIndexIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandIndexIntermediate"));
					MotionCaptureFrame.RightHandIndexDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandIndexDistal"));

					MotionCaptureFrame.LeftHandMiddleProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandMiddleProximal"));
					MotionCaptureFrame.LeftHandMiddleIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandMiddleIntermediate"));
					MotionCaptureFrame.LeftHandMiddleDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandMiddleDistal"));

					MotionCaptureFrame.RightHandMiddleProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandMiddleProximal"));
					MotionCaptureFrame.RightHandMiddleIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandMiddleIntermediate"));
					MotionCaptureFrame.RightHandMiddleDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandMiddleDistal"));

					MotionCaptureFrame.LeftHandRingProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandRingProximal"));
					MotionCaptureFrame.LeftHandRingIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandRingIntermediate"));
					MotionCaptureFrame.LeftHandRingDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandRingDistal"));

					MotionCaptureFrame.RightHandRingProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandRingProximal"));
					MotionCaptureFrame.RightHandRingIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandRingIntermediate"));
					MotionCaptureFrame.RightHandRingDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandRingDistal"));

					MotionCaptureFrame.LeftHandPinkyMetacarpal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandPinkyMetacarpal"));
					MotionCaptureFrame.LeftHandPinkyProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandPinkyProximal"));
					MotionCaptureFrame.LeftHandPinkyIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandPinkyIntermediate"));
					MotionCaptureFrame.LeftHandPinkyDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("leftHandPinkyDistal"));

					MotionCaptureFrame.RightHandPinkyMetacarpal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandPinkyMetacarpal"));
					MotionCaptureFrame.RightHandPinkyProximal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandPinkyProximal"));
					MotionCaptureFrame.RightHandPinkyIntermediate = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandPinkyIntermediate"));
					MotionCaptureFrame.RightHandPinkyDistal = GetTransformFromJsonObject(JsonObjFrame->GetObjectField("rightHandPinkyDistal"));
					
					Frames.Add(MotionCaptureFrame);
				}

				Capture.Frames = Frames;

				// Parse/create
			}	
		}
	}
}

FTransform FVaraHttpServer::GetTransformFromJsonObject(TSharedPtr<FJsonObject> JsonObject)
{
	FTransform Transform;

	Transform.SetTranslation(GetVectorFromJsonObject(JsonObject->GetObjectField("translation")));
	Transform.SetRotation(GetQuatFromJsonObject(JsonObject->GetObjectField("rotation")));
	Transform.SetScale3D(GetVectorFromJsonObject(JsonObject->GetObjectField("scale3D")));

	return Transform;
}

FVector FVaraHttpServer::GetVectorFromJsonObject(TSharedPtr<FJsonObject> JsonObject)
{
	FVector Vector;

	Vector.X = JsonObject->GetNumberField("x");
	Vector.Y = JsonObject->GetNumberField("y");
	Vector.Z = JsonObject->GetNumberField("z");

	return Vector;
}

FQuat FVaraHttpServer::GetQuatFromJsonObject(TSharedPtr<FJsonObject> JsonObject)
{
	FQuat Quat;

	Quat.X = JsonObject->GetNumberField("x");
	Quat.Y = JsonObject->GetNumberField("y");
	Quat.Z = JsonObject->GetNumberField("z");
	Quat.W = JsonObject->GetNumberField("w");

	return Quat;
}
