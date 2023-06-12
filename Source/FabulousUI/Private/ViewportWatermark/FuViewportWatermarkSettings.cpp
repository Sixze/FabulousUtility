#include "ViewportWatermark/FuViewportWatermarkSettings.h"

#include "HardwareInfo.h"
#include "RHI.h"
#include "Engine/Font.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include "Launch/Resources/Version.h"
#include "UObject/ConstructorHelpers.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(FuViewportWatermarkSettings)

#define LOCTEXT_NAMESPACE "FuViewportWatermarkSettings"

UFuViewportWatermarkSettings::UFuViewportWatermarkSettings()
{
	CategoryName = FName{TEXTVIEW("Fabulous Utility")};

	TitleText = LOCTEXT("Title", "WORK IN PROGRESS - DOES NOT REPRESENT THE FINAL LOOK OF THE GAME");

	CopyrightText = LOCTEXT("Copyright", "Created by <Name>");

	EngineVersionText = FText::Format(LOCTEXT("EngineVersion", "Built using Unreal Engine {EngineVersion}"),
	                                  {
		                                  {FString{TEXTVIEW("EngineVersion")}, FText::AsCultureInvariant(ENGINE_VERSION_STRING)}
	                                  });

	SystemInfoText =
		FText::Format(
			LOCTEXT("SystemInfo",
			        "{CpuBrand} ({CpuCores} Cores)" LINE_TERMINATOR
			        "{GpuBrand}" LINE_TERMINATOR
			        "RHI: {Rhi} Driver: {GpuDriver}"),
			{
				{FString{TEXTVIEW("CpuBrand")}, FText::AsCultureInvariant(FPlatformMisc::GetCPUBrand())},
				{FString{TEXTVIEW("CpuCores")}, FText::AsNumber(FPlatformMisc::NumberOfCores())},
				{FString{TEXTVIEW("GpuBrand")}, FText::AsCultureInvariant(GRHIAdapterName)},
				{FString{TEXTVIEW("Rhi")}, FText::AsCultureInvariant(FHardwareInfo::GetHardwareInfo(NAME_RHI))},
				{
					FString{TEXTVIEW("GpuDriver")},
					FText::AsCultureInvariant(FPlatformMisc::GetGPUDriverInfo(GRHIAdapterName).UserDriverVersion)
				}
			});

	TitleSettings.HorizontalAlignment = HAlign_Center;
	TitleSettings.VerticalAlignment = VAlign_Top;
	TitleSettings.Padding = {0.0f, 100.0f};

	CopyrightSettings.VerticalAlignment = VAlign_Bottom;
	CopyrightSettings.HorizontalAlignment = HAlign_Left;

	SystemInfoSettings.VerticalAlignment = VAlign_Bottom;
	SystemInfoSettings.HorizontalAlignment = HAlign_Right;

	if (!IsRunningDedicatedServer())
	{
		static const ConstructorHelpers::FObjectFinder<UFont> RobotoFont{TEXT("/Engine/EngineFonts/Roboto")};

		TitleSettings.Font = FSlateFontInfo{RobotoFont.Object, 32, FName{TEXTVIEW("Bold")}};
		CopyrightSettings.Font = FSlateFontInfo{RobotoFont.Object, 24, FName{TEXTVIEW("Bold")}};
		SystemInfoSettings.Font = FSlateFontInfo{RobotoFont.Object, 24, FName{TEXTVIEW("Bold")}};
	}
}

#if WITH_EDITOR
FText UFuViewportWatermarkSettings::GetSectionText() const
{
	return LOCTEXT("Section", "Viewport Watermark");
}
#endif

#undef LOCTEXT_NAMESPACE
