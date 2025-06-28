using UnrealBuildTool;

public class FabulousAI : ModuleRules
{
	public FabulousAI(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "GameplayTags", "AIModule"
		]);

		PrivateDependencyModuleNames.AddRange([
			"GameplayAbilities", "FabulousUtility", "FabulousAbilitySystem"
		]);
	}
}