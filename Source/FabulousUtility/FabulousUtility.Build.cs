using UnrealBuildTool;

public class FabulousUtility : ModuleRules
{
	public FabulousUtility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "NetCore", "EngineSettings", "RHI", "InputCore",
			"SlateCore", "Slate", "OnlineSubsystem", "OnlineSubsystemUtils", "GameplayTags",
			"GameplayTasks", "GameplayAbilities", "NavigationSystem", "AIModule", "DeveloperSettings"
		});
	}
}