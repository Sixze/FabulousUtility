using UnrealBuildTool;

public class FabulousAnimation : ModuleRules
{
	public FabulousAnimation(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
			"GameplayTags", "AnimGraphRuntime", "GameplayAbilities", "FabulousUtility", "FabulousAbilitySystem"
		]);
	}
}