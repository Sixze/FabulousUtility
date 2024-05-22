#pragma once

#include "Engine/DeveloperSettings.h"
#include "Fonts/SlateFontInfo.h"
#include "Types/SlateEnums.h"
#include "FuViewportWatermarkSettings.generated.h"

USTRUCT(BlueprintType)
struct FABULOUSUI_API FFuViewportWatermarkTextSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment{HAlign_Center};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment{VAlign_Center};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI", Meta = (AllowPreserveRatio))
	FVector2f Padding{50.0f, 50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	FLinearColor ColorAndOpacity{0.8f, 0.8f, 0.8f, 0.2f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	FVector2f ShadowOffset{FVector2f::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	FLinearColor ShadowColorAndOpacity{0.0f, 0.0f, 0.0f, 0.0f};
};

UCLASS(Config = "Engine", DefaultConfig)
class FABULOUSUI_API UFuViewportWatermarkSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config)
	uint8 bEnabled : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewport Watermark", Config, DisplayName = "Z Order")
	int32 ZOrder{100};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title", Config)
	uint8 bShowTitle : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title", Config, Meta = (MultiLine = true, EditCondition = "bShowTitle"))
	FText TitleText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title", Config, Meta = (EditCondition = "bShowTitle"))
	FFuViewportWatermarkTextSettings TitleSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copyright", Config)
	uint8 bShowCopyright : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copyright", Config, Meta = (MultiLine = true, EditCondition = "bShowCopyright"))
	FText CopyrightText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copyright", Config, Meta = (EditCondition = "bShowCopyright"))
	uint8 bAddEngineVersionToCopyright : 1 {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Copyright", Transient, Meta = (MultiLine = true))
	FText EngineVersionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Copyright", Config, Meta = (EditCondition = "bShowCopyright"))
	FFuViewportWatermarkTextSettings CopyrightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System Info", Config)
	uint8 bShowSystemInfo : 1 {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System Info", Transient, Meta = (MultiLine = true))
	FText SystemInfoText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System Info", Config, Meta = (EditCondition = "bShowSystemInfo"))
	FFuViewportWatermarkTextSettings SystemInfoSettings;

public:
	UFuViewportWatermarkSettings();

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
};
