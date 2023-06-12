using UnrealBuildTool;

public class FabulousAnimation : ModuleRules
{
	public FabulousAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

		bEnableNonInlinedGenCppWarnings = true;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "GameplayTags", "GameplayAbilities", "FabulousUtility", "FabulousAbilitySystem"
		});
	}
}