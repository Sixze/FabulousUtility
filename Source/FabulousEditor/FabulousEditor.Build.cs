using UnrealBuildTool;

public class FabulousEditor : ModuleRules
{
	public FabulousEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

		bEnableNonInlinedGenCppWarnings = true;

		PrivateDependencyModuleNames.AddRange(new[] {"Core", "CoreUObject", "Slate", "DeveloperSettings"});
	}
}