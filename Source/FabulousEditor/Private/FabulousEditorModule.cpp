#include "FabulousEditorModule.h"

#include "FuEditorSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FFabulousEditorModule, FabulousEditor)

#define LOCTEXT_NAMESPACE "FabulousEditorModule"

void FFabulousEditorModule::StartupModule()
{
	IModuleInterface::StartupModule();

	auto* Settings{GetMutableDefault<UFuEditorSettings>()};

	if (!Settings->bApplyEditorScale || !FSlateApplication::IsInitialized())
	{
		return;
	}

	if (Settings->EditorScale < 0.5f || Settings->EditorScale > 2.0f)
	{
		Settings->EditorScale = FMath::Clamp(Settings->EditorScale, 0.5f, 2.0f);

		Settings->UpdateSinglePropertyInConfigFile(
			UFuEditorSettings::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UFuEditorSettings, EditorScale)),
			Settings->GetDefaultConfigFilename());
	}

	FSlateApplication::Get().SetApplicationScale(Settings->EditorScale);
}

void FFabulousEditorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE
