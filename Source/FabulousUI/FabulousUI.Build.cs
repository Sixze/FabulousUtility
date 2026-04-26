using System.Linq;
using EpicGames.Core;
using Microsoft.Extensions.Logging;
using UnrealBuildTool;

public class FabulousUI : ModuleRules
{
	public FabulousUI(ReadOnlyTargetRules target) : base(target)
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
			"Core", "CoreUObject", "Engine", "DeveloperSettings", "InputCore", "SlateCore", "UMG", "CommonInput", "CommonUI"
		]);

		PrivateDependencyModuleNames.AddRange([
			"GameplayTags", "RHI", "Slate", "EnhancedInput", "FabulousUtility"
		]);

		if (IsPluginEnabled(target, "ImGui"))
		{
			PublicDependencyModuleNames.Add("ImGui");
			PublicDefinitions.Add("WITH_IMGUI");
		}
		else
		{
			Logger.LogInformation("Unable to find the 'ImGui for Unreal Engine' plugin. " +
			                      "Integration of 'Fabulous Utility' with 'Dear ImGui' will be disabled.");
		}
	}

	private static bool IsPluginEnabled(ReadOnlyTargetRules target, string pluginName) =>
		target.ProjectFile != null &&
		JsonObject.TryRead(target.ProjectFile, out var projectJson) &&
		projectJson.TryGetObjectArrayField("Plugins", out var pluginsJson) &&
		pluginsJson.Any(pluginJson => pluginJson.TryGetBoolField("Enabled", out var bEnabled) && bEnabled &&
		                              pluginJson.TryGetStringField("Name", out var name) && name == pluginName);
}