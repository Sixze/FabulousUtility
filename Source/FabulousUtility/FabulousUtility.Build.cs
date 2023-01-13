using UnrealBuildTool;

public class FabulousUtility : ModuleRules
{
	public FabulousUtility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", "EnhancedInput", "NetCore", "RHI",
			"SlateCore", "Slate", "UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "GameplayTags", "GameplayTasks",
			"GameplayAbilities", "CommonInput", "CommonUI", "NavigationSystem", "AIModule", "DeveloperSettings"
		});
	}
}