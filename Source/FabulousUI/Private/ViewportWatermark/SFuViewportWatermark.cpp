#include "ViewportWatermark/SFuViewportWatermark.h"

#include "Engine/UserInterfaceSettings.h"
#include "ViewportWatermark/FuViewportWatermarkSettings.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Text/STextBlock.h"

SFuViewportWatermark::SFuViewportWatermark()
{
	bCanSupportFocus = false;

	SetCanTick(false);
}

void SFuViewportWatermark::Construct(const FArguments& Arguments)
{
	const auto* ViewportWatermarkSettings{GetDefault<UFuViewportWatermarkSettings>()};

	const auto Overlay{SNew(SOverlay)};

	if (ViewportWatermarkSettings->bShowTitle)
	{
		Overlay->AddSlot()
		       .HAlign(ViewportWatermarkSettings->TitleSettings.HorizontalAlignment)
		       .VAlign(ViewportWatermarkSettings->TitleSettings.VerticalAlignment)
		       .Padding(ViewportWatermarkSettings->TitleSettings.Padding)
		[
			SNew(STextBlock)
			.Text(ViewportWatermarkSettings->TitleText)
			.Font(ViewportWatermarkSettings->TitleSettings.Font)
			.ColorAndOpacity(ViewportWatermarkSettings->TitleSettings.ColorAndOpacity)
			.ShadowOffset(ViewportWatermarkSettings->TitleSettings.ShadowOffset)
			.ShadowColorAndOpacity(ViewportWatermarkSettings->TitleSettings.ShadowColorAndOpacity)
		];
	}

	if (ViewportWatermarkSettings->bShowCopyright)
	{
		Overlay->AddSlot()
		       .HAlign(ViewportWatermarkSettings->CopyrightSettings.HorizontalAlignment)
		       .VAlign(ViewportWatermarkSettings->CopyrightSettings.VerticalAlignment)
		       .Padding(ViewportWatermarkSettings->CopyrightSettings.Padding)
		[
			SNew(STextBlock)
			.Text(ViewportWatermarkSettings->bAddEngineVersionToCopyright
				      ? FText::Format(FText::AsCultureInvariant(FString{TEXTVIEW("{0}") LINE_TERMINATOR TEXTVIEW("{1}")}), {
					                      {ViewportWatermarkSettings->CopyrightText},
					                      {ViewportWatermarkSettings->EngineVersionText}
				                      })
				      : ViewportWatermarkSettings->CopyrightText)
			.Font(ViewportWatermarkSettings->CopyrightSettings.Font)
			.ColorAndOpacity(ViewportWatermarkSettings->CopyrightSettings.ColorAndOpacity)
			.ShadowOffset(ViewportWatermarkSettings->CopyrightSettings.ShadowOffset)
			.ShadowColorAndOpacity(ViewportWatermarkSettings->CopyrightSettings.ShadowColorAndOpacity)
		];
	}

	if (ViewportWatermarkSettings->bShowSystemInfo)
	{
		Overlay->AddSlot()
		       .HAlign(ViewportWatermarkSettings->SystemInfoSettings.HorizontalAlignment)
		       .VAlign(ViewportWatermarkSettings->SystemInfoSettings.VerticalAlignment)
		       .Padding(ViewportWatermarkSettings->SystemInfoSettings.Padding)
		[
			SNew(STextBlock)
			.Text(ViewportWatermarkSettings->SystemInfoText)
			.Font(ViewportWatermarkSettings->SystemInfoSettings.Font)
			.ColorAndOpacity(ViewportWatermarkSettings->SystemInfoSettings.ColorAndOpacity)
			.ShadowOffset(ViewportWatermarkSettings->SystemInfoSettings.ShadowOffset)
			.ShadowColorAndOpacity(ViewportWatermarkSettings->SystemInfoSettings.ShadowColorAndOpacity)
		];
	}

	const auto* UserInterfaceSettings{GetDefault<UUserInterfaceSettings>()};

	ChildSlot
	[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors{0.5f, 0.0f, 0.5f, 1.0f})
		.Offset(FMargin{0.0f, 0.0f, static_cast<float>(UserInterfaceSettings->DesignScreenSize.X), 0.0f})
		.Alignment(FVector2f{0.5f, 0.5f})
		[
			Overlay
		]
	];
}
