// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VaraMocap : ModuleRules
{
	public VaraMocap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "VaraCommon", "LibOVRPlatform", "OculusXRMovement", "Json", "JsonUtilities", "HTTP" });

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
