using UnrealBuildTool;

public class FabulousOnline : ModuleRules
{
	public FabulousOnline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

		bEnableNonInlinedGenCppWarnings = true;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "OnlineSubsystem", "OnlineSubsystemUtils", "FabulousUtility"
		});
	}
}