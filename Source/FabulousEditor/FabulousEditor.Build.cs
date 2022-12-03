using UnrealBuildTool;

public class FabulousEditor : ModuleRules
{
	public FabulousEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[] {"Core", "CoreUObject", "Engine", "Slate", "DeveloperSettings"});
	}
}