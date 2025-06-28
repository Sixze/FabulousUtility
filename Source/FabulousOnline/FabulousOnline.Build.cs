using UnrealBuildTool;

public class FabulousOnline : ModuleRules
{
	public FabulousOnline(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "OnlineSubsystemUtils"
		]);

		PrivateDependencyModuleNames.AddRange([
			"OnlineSubsystem", "FabulousUtility"
		]);
	}
}