using UnrealBuildTool;

public class FabulousUtility : ModuleRules
{
	public FabulousUtility(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
			"EngineSettings", "NetCore", "SlateCore", "GameplayTags"
		]);

		SetupIrisSupport(target);
	}
}