using UnrealBuildTool;

public class FabulousUI : ModuleRules
{
	public FabulousUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;

		bEnableNonInlinedGenCppWarnings = true;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "SlateCore", "UMG", "CommonInput", "CommonUI", "DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"EnhancedInput", "RHI", "Slate", "GameplayTags", "FabulousUtility"
		});
	}
}