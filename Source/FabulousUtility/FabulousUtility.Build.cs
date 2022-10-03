using UnrealBuildTool;

public class FabulousUtility : ModuleRules
{
	public FabulousUtility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "EngineSettings", "InputCore", "EnhancedInput", "NetCore", "RHI",
			"SlateCore", "Slate", "OnlineSubsystem", "OnlineSubsystemUtils", "GameplayTags", "GameplayTasks",
			"GameplayAbilities", "CommonUI", "NavigationSystem", "AIModule", "DeveloperSettings"
		});
	}
}