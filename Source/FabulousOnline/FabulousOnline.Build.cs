using UnrealBuildTool;

public class FabulousOnline : ModuleRules
{
	public FabulousOnline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;

		bEnableNonInlinedGenCppWarnings = true;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "OnlineSubsystemUtils"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"OnlineSubsystem", "FabulousUtility"
		});
	}
}