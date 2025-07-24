using UnrealBuildTool;

public class FabulousAbilitySystem : ModuleRules
{
	public FabulousAbilitySystem(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "GameplayTags", "GameplayAbilities", "FabulousUtility"
		]);

		PrivateDependencyModuleNames.AddRange([
			"EnhancedInput", "NetCore", "GameplayTasks", "NavigationSystem"
		]);

		SetupIrisSupport(target);
	}
}