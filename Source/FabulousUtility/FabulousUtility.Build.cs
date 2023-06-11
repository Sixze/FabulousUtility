using UnrealBuildTool;

public class FabulousUtility : ModuleRules
{
	public FabulousUtility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

		bEnableNonInlinedGenCppWarnings = true;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", "NetCore", "RHI", "SlateCore", "Slate",
			"UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "GameplayTags", "GameplayTasks", "GameplayAbilities",
			"CommonInput", "CommonUI", "NavigationSystem", "AIModule", "DeveloperSettings", "FabulousAbilitySystem"
		});
	}
}