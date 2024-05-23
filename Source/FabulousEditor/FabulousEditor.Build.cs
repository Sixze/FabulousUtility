using UnrealBuildTool;

public class FabulousEditor : ModuleRules
{
	public FabulousEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;

		bEnableNonInlinedGenCppWarnings = true;
		// UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Slate", "DeveloperSettings"
		});
	}
}