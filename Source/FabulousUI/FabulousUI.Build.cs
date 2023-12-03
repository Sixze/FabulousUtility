using UnrealBuildTool;

public class FabulousUI : ModuleRules
{
	public FabulousUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;

		bEnableNonInlinedGenCppWarnings = true;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "RHI", "SlateCore", "Slate",
			"UMG", "GameplayTags", "CommonInput", "CommonUI", "FabulousUtility", "DeveloperSettings"
		});
	}
}