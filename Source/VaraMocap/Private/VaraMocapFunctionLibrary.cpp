// All Rights Reserved


#include "VaraMocapFunctionLibrary.h"
#include "JsonObjectConverter.h"
#include "VaraConfig.h"

void UVaraMocapFunctionLibrary::BodyCaptureToJsonString(const FVaraMotionCapture& InCapture, FString& OutJsonString)
{
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	
	JsonObject->SetObjectField("capture", FJsonObjectConverter::UStructToJsonObject(InCapture));

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	FJsonSerializer::Serialize(JsonObject, Writer);
}

FString UVaraMocapFunctionLibrary::GetBaseUrl()
{
	return FVaraConfig::GetIpAddr();
}
