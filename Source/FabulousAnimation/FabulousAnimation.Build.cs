using UnrealBuildTool;

public class FabulousAnimation : ModuleRules
{
	public FabulousAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;

		bEnableNonInlinedGenCppWarnings = true;
		// UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"GameplayTags", "AnimGraphRuntime", "GameplayAbilities", "FabulousUtility", "FabulousAbilitySystem"
		});
	}
}