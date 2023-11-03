using UnrealBuildTool;

public class VaraCommon : ModuleRules
{
	public VaraCommon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Json", 
				"JsonUtilities", 
				"HTTP",
				"Engine"
			}
		);
	}
}