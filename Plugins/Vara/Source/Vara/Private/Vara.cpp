// Copyright Epic Games, Inc. All Rights Reserved.

#include "Vara.h"

#include "ContentBrowserModule.h"
#include "VaraStyle.h"
#include "VaraCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/SavePackage.h"
#include "IContentBrowserSingleton.h"
#include "VaraHttpServer.h"

static const FName VaraTabName("Vara");

#define LOCTEXT_NAMESPACE "FVaraModule"

void FVaraModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FVaraStyle::Initialize();
	FVaraStyle::ReloadTextures();

	FVaraCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FVaraCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FVaraModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FVaraModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(VaraTabName, FOnSpawnTab::CreateRaw(this, &FVaraModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FVaraTabTitle", "Vara"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FVaraModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FVaraStyle::Shutdown();

	FVaraCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(VaraTabName);

	if (HttpServer)
	{
		HttpServer->Stop();
		delete HttpServer;
	}
}

TSharedRef<SDockTab> FVaraModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FVaraModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Vara.cpp"))
		);

	// CreateAnimation();
	check(!HttpServer)
	HttpServer = new FVaraHttpServer;
	check(HttpServer);

	HttpServer->OnServerEvent.BindLambda([this](FVaraMotionCapture Capture)
	{
		CreateAnimation(Capture);
	});
	
	HttpServer->Start();
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FVaraModule::CreateAnimation(const FVaraMotionCapture& Capture)
{
	FStringAssetReference SkeletonReference(TEXT("/Vara/Proteus/proteus_head_LOD2_and_body_rig_Skeleton1"));
	USkeleton* Skeleton = Cast<USkeleton>(SkeletonReference.TryLoad());
	check(IsValid(Skeleton));
	
	// Create a new animation asset
    FName NewAssetName = FName(*GetRandomName());
    UAnimSequence* NewAnimation = NewObject<UAnimSequence>(GetTransientPackage(), NewAssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	
	check(NewAnimation);
	
    if (NewAnimation)
    {
        // Attach the selected skeleton to the animation
		NewAnimation->SetSkeleton(Skeleton);

		IAnimationDataController& Controller = NewAnimation->GetController();
		Controller.InitializeModel(); // This is how to get around the break in UE5.2!
    	
		const float PlayLength = Capture.Frames.Last().Timestamp - Capture.Frames[0].Timestamp;
    	const int32 NumFrames = Capture.Frames.Num();
    	
    	Controller.OpenBracket(FText::FromString(FString(TEXT("Anim Populating"))), true);
    	
		Controller.SetFrameRate(FFrameRate(NumFrames, PlayLength));
    	Controller.SetNumberOfFrames(FFrameNumber(NumFrames));
    	Controller.NotifyPopulated();
    	
    	Controller.RemoveAllBoneTracks();

		FReferenceSkeleton RefSkeleton = Skeleton->GetReferenceSkeleton();
    	const TArray<FTransform>& RefPoses = RefSkeleton.GetRefBonePose();
		const int32 NumBones = RefSkeleton.GetNum();
		for (int32 BoneIndex = 0; BoneIndex < NumBones; BoneIndex++)
		{
			const FName BoneName = RefSkeleton.GetBoneName(BoneIndex);
			const bool bAddSuccess = Controller.AddBoneCurve(BoneName);
			check(bAddSuccess);

			TArray<FVector> PosKeys;
			TArray<FQuat> RotKeys;
			TArray<FVector> ScaleKeys;

			PosKeys.Init(RefPoses[BoneIndex].GetLocation(), Controller.GetModel()->GetNumberOfFrames() + 1);
			RotKeys.Init(RefPoses[BoneIndex].GetRotation(), Controller.GetModel()->GetNumberOfFrames() + 1);
			ScaleKeys.Init(RefPoses[BoneIndex].GetScale3D(), Controller.GetModel()->GetNumberOfFrames() + 1);

			const bool bTrackSuccess = Controller.SetBoneTrackKeys(BoneName, PosKeys, RotKeys, ScaleKeys);
			check(bTrackSuccess);
			
			for (int32 FrameIndex = 0; FrameIndex < NumFrames; FrameIndex++)
			{
				// Get the bone transform for this frame
				FTransform BoneTransform;
				
				const FVaraMotionCaptureFrame& FrameData = Capture.Frames[FrameIndex];
				
				if (BoneName == "Root")
				{
					// BoneTransform = FrameData.Root;
				}
				else if (BoneName == "Hips")
				{
					BoneTransform = FrameData.Hips;
				}
				else if (BoneName == "SpineLower")
				{
					BoneTransform = FrameData.SpineLower;
				}
				else if (BoneName == "SpineMiddle")
				{
					BoneTransform = FrameData.SpineMiddle;
				}
				else if (BoneName == "SpineUpper")
				{
					BoneTransform = FrameData.SpineUpper;
				}
				else if (BoneName == "Chest")
				{
					BoneTransform = FrameData.Chest;
				}
				else if (BoneName == "Neck")
				{
					BoneTransform = FrameData.Neck;
				}
				else if (BoneName == "Head")
				{
					BoneTransform = FrameData.Head;
				}
				// Body
				else if (BoneName == "LeftShoulder")
				{
					BoneTransform = FrameData.LeftShoulder;
				}
				else if (BoneName == "LeftArmUpper")
				{
					BoneTransform = FrameData.LeftArmUpper;
				}
				else if (BoneName == "LeftArmLower")
				{
					BoneTransform = FrameData.LeftArmLower;
				}
				else if (BoneName == "LeftHandWrist")
				{
					BoneTransform = FrameData.LeftHandWrist;
				}
				else if (BoneName == "RightShoulder")
				{
					BoneTransform = FrameData.RightShoulder;
				}
				else if (BoneName == "RightArmUpper")
				{
					BoneTransform = FrameData.RightArmUpper;
				}
				else if (BoneName == "RightArmLower")
				{
					BoneTransform = FrameData.RightArmLower;
				}
				else if (BoneName == "RightHandWrist")
				{
					BoneTransform = FrameData.RightHandWrist;
				}
				/// Hands
				// thumbs
				else if (BoneName == "LeftHandThumbMetacarpal")
				{
					BoneTransform = FrameData.LeftHandThumbMetacarpal;
				}
				else if (BoneName == "LeftHandThumbProximal")
				{
					BoneTransform = FrameData.LeftHandThumbProximal;
				}
				else if (BoneName == "LeftHandThumbDistal")
				{
					BoneTransform = FrameData.LeftHandThumbDistal;
				}
				else if (BoneName == "RightHandThumbMetacarpal")
				{
					BoneTransform = FrameData.RightHandThumbMetacarpal;
				}
				else if (BoneName == "RightHandThumbProximal")
				{
					BoneTransform = FrameData.RightHandThumbProximal;
				}
				else if (BoneName == "RightHandThumbDistal")
				{
					BoneTransform = FrameData.RightHandThumbDistal;
				}
				// index
				else if (BoneName == "LeftHandIndexProximal")
				{
					BoneTransform = FrameData.LeftHandIndexProximal;
				}
				else if (BoneName == "LeftHandIndexIntermediate")
				{
					BoneTransform = FrameData.LeftHandIndexIntermediate;
				}
				else if (BoneName == "LeftHandIndexDistal")
				{
					BoneTransform = FrameData.LeftHandIndexDistal;
				}
				else if (BoneName == "RightHandIndexProximal")
				{
					BoneTransform = FrameData.RightHandIndexProximal;
				}
				else if (BoneName == "RightHandIndexIntermediate")
				{
					BoneTransform = FrameData.RightHandIndexIntermediate;
				}
				else if (BoneName == "RightHandIndexDistal")
				{
					BoneTransform = FrameData.RightHandIndexDistal;
				}
				// middle
				else if (BoneName == "LeftHandMiddleProximal")
				{
					BoneTransform = FrameData.LeftHandMiddleProximal;
				}
				else if (BoneName == "LeftHandMiddleIntermediate")
				{
					BoneTransform = FrameData.LeftHandMiddleIntermediate;
				}
				else if (BoneName == "LeftHandMiddleDistal")
				{
					BoneTransform = FrameData.LeftHandMiddleDistal;
				}
				else if (BoneName == "RightHandMiddleProximal")
				{
					BoneTransform = FrameData.RightHandMiddleProximal;
				}
				else if (BoneName == "RightHandMiddleIntermediate")
				{
					BoneTransform = FrameData.RightHandMiddleIntermediate;
				}
				else if (BoneName == "RightHandMiddleDistal")
				{
					BoneTransform = FrameData.RightHandMiddleDistal;
				}
				// ring
				else if (BoneName == "LeftHandRingProximal")
				{
					BoneTransform = FrameData.LeftHandRingProximal;
				}
				else if (BoneName == "LeftHandRingIntermediate")
				{
					BoneTransform = FrameData.LeftHandRingIntermediate;
				}
				else if (BoneName == "LeftHandRingDistal")
				{
					BoneTransform = FrameData.LeftHandRingDistal;
				}
				else if (BoneName == "RightHandRingProximal")
				{
					BoneTransform = FrameData.RightHandRingProximal;
				}
				else if (BoneName == "RightHandRingIntermediate")
				{
					BoneTransform = FrameData.RightHandRingIntermediate;
				}
				else if (BoneName == "RightHandRingDistal")
				{
					BoneTransform = FrameData.RightHandRingDistal;
				}
				// pinky
				else if (BoneName == "LeftHandPinkyMetacarpal")
				{
					BoneTransform = FrameData.LeftHandPinkyMetacarpal;
				}
				else if (BoneName == "LeftHandPinkyProximal")
				{
					BoneTransform = FrameData.LeftHandPinkyProximal;
				}
				else if (BoneName == "LeftHandPinkyIntermediate")
				{
					BoneTransform = FrameData.LeftHandPinkyIntermediate;
				}
				else if (BoneName == "LeftHandPinkyDistal")
				{
					BoneTransform = FrameData.LeftHandPinkyDistal;
				}
				else if (BoneName == "RightHandPinkyMetacarpal")
				{
					BoneTransform = FrameData.RightHandPinkyMetacarpal;
				}
				else if (BoneName == "RightHandPinkyProximal")
				{
					BoneTransform = FrameData.RightHandPinkyProximal;
				}
				else if (BoneName == "RightHandPinkyIntermediate")
				{
					BoneTransform = FrameData.RightHandPinkyIntermediate;
				}
				else if (BoneName == "RightHandPinkyDistal")
				{
					BoneTransform = FrameData.RightHandPinkyDistal;
				}
				
				if (BoneTransform.GetLocation().X == 0 && BoneTransform.GetLocation().Y == 0 && BoneTransform.GetLocation().Z == 0)
				{
					// Don't track non-set bones
					// UE_LOG(LogTemp, Warning, TEXT("NOT TRACKING %s"), *BoneName.ToString());
				}
				else
				{
					PosKeys[FrameIndex] = BoneTransform.GetLocation();
					RotKeys[FrameIndex] = BoneTransform.GetRotation();
					ScaleKeys[FrameIndex] = BoneTransform.GetScale3D();	
				}
			}

			const bool bFinalSuccess = Controller.SetBoneTrackKeys(BoneName, PosKeys, RotKeys, ScaleKeys);
			check(bFinalSuccess);
		}

    	Controller.CloseBracket(true);
    	Controller.NotifyPopulated();
    	
    	FString PackagePath; // = FString(TEXT("/Content/"));
    	PackagePath = FPaths::ProjectContentDir();

    	// TODO: Need to handle duplicate names better
    	FString AssetName = NewAssetName.ToString();
        UPackage* AssetPackage = CreatePackage(*(PackagePath + AssetName));
        NewAnimation->Rename(*AssetName, AssetPackage);
        AssetPackage->AddToRoot();
        AssetPackage->FullyLoad();
        AssetPackage->MarkPackageDirty();
        NewAnimation->MarkPackageDirty();
        
        FString SavePath = PackagePath + AssetName + TEXT(".uasset"); // Save as a .uasset file

    	FSavePackageArgs PackageArgs = FSavePackageArgs();
    	PackageArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
    	PackageArgs.SaveFlags = SAVE_None;
    	const bool bSaveSuccess = UPackage::SavePackage(AssetPackage, NewAnimation, *SavePath, PackageArgs);
    	check(bSaveSuccess)

    	// Open animation window
    	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
    	if (AssetEditorSubsystem)
    	{
    		TArray<FString> ObjectsToOpen;
    		ObjectsToOpen.Add(SavePath);
    		AssetEditorSubsystem->OpenEditorsForAssets(ObjectsToOpen);
    	}
    	
    	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    	TArray<FAssetData> AssetDatas;
    	FAssetData AssetData = FAssetData(NewAnimation);
    	AssetDatas.Add(AssetData);
    	ContentBrowserModule.Get().SyncBrowserToAssets(AssetDatas, true, true, FName(), false);
    }
}

FString FVaraModule::GetRandomName() const
{
	const int32 NameLength = 4;
	const FString AllowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const int32 NumAllowedCharacters = AllowedCharacters.Len();

	FString RandomName;

	// Generate a random name by choosing characters from the allowed characters string
	for (int32 i = 0; i < NameLength; i++)
	{
		const int32 RandomIndex = FMath::RandRange(0, NumAllowedCharacters - 1);
		RandomName += AllowedCharacters[RandomIndex];
	}

	return (FString(TEXT("Vara")) + RandomName);
}

void FVaraModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(VaraTabName);
}

void FVaraModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FVaraCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FVaraCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVaraModule, Vara)