using UnrealBuildTool;

public class FabulousAI : ModuleRules
{
	public FabulousAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

		bEnableNonInlinedGenCppWarnings = true;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "GameplayTags", "GameplayTasks", "GameplayAbilities",
			"NavigationSystem", "AIModule", "FabulousUtility", "FabulousAbilitySystem"
		});
	}
}