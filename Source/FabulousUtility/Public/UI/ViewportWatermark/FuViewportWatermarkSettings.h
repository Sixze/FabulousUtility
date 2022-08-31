#pragma once

#include "Engine/DeveloperSettings.h"
#include "Fonts/SlateFontInfo.h"
#include "Types/SlateEnums.h"
#include "FuViewportWatermarkSettings.generated.h"

USTRUCT(BlueprintType)
struct FABULOUSUTILITY_API FFuViewportWatermarkTextSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment{HAlign_Center};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment{VAlign_Center};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility", Meta = (AllowPreserveRatio))
	FVector2D Padding{50.0f, 50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FLinearColor ColorAndOpacity{0.8f, 0.8f, 0.8f, 0.2f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility", Meta = (AllowPreserveRatio))
	FVector2D ShadowOffset{FVector2D::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FLinearColor ShadowColorAndOpacity{0.0f, 0.0f, 0.0f, 0.0f};
};

UCLASS(Config = "Engine", DefaultConfig)
class FABULOUSUTILITY_API UFuViewportWatermarkSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config)
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config)
	bool bShowTitle{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config, Meta = (MultiLine = "true"))
	FText TitleText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config)
	bool bShowCopyright{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config, Meta = (MultiLine = "true"))
	FText CopyrightText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config)
	bool bAddEngineVersionToCopyright{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Transient, Meta = (MultiLine = "true"))
	FText EngineVersionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config)
	bool bShowSystemInfo{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Transient, Meta = (MultiLine = "true"))
	FText SystemInfoText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals", Config, DisplayName = "Widget Z Order")
	int32 WidgetZOrder{1000};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals", Config)
	FFuViewportWatermarkTextSettings TitleSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals", Config)
	FFuViewportWatermarkTextSettings CopyrightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals", Config)
	FFuViewportWatermarkTextSettings SystemInfoSettings;

public:
	UFuViewportWatermarkSettings();

#if WITH_EDITOR
	virtual FText GetSectionText() const override final;
#endif
};
