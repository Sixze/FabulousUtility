using UnrealBuildTool;

public class FabulousAI : ModuleRules
{
	public FabulousAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;

		bEnableNonInlinedGenCppWarnings = true;
		// UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "GameplayTags", "AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"GameplayAbilities", "FabulousUtility", "FabulousAbilitySystem"
		});
	}
}