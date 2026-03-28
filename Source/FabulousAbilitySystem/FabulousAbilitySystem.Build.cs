using UnrealBuildTool;

public class FabulousAbilitySystem : ModuleRules
{
	public FabulousAbilitySystem(ReadOnlyTargetRules target) : base(target)
	{
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		// if (Target.bBuildEditor)
		// {
		// 	// Verify that all source files include all required dependencies.
		//
		// 	bUseUnity = false;
		// 	PCHUsage = PCHUsageMode.NoPCHs;
		// }

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "GameplayTags", "GameplayAbilities", "FabulousUtility"
		]);

		PrivateDependencyModuleNames.AddRange([
			"NetCore", "NavigationSystem", "GameplayTasks", "EnhancedInput"
		]);
	}
}