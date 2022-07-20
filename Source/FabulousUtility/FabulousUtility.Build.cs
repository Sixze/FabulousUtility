using UnrealBuildTool;

public class FabulousUtility : ModuleRules
{
	public FabulousUtility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "SlateCore", "OnlineSubsystem", "OnlineSubsystemUtils",
			"GameplayTags", "GameplayTasks", "GameplayAbilities", "NavigationSystem", "AIModule", "DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"NetCore", "RHI", "Slate", "EngineSettings"
		});
	}
}