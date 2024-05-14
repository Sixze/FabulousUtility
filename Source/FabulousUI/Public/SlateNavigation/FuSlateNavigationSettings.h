#pragma once

#include "InputCoreTypes.h"
#include "Engine/DeveloperSettings.h"
#include "FuSlateNavigationSettings.generated.h"

enum class EUINavigation : uint8;

UCLASS(Config = "Engine", DefaultConfig)
class FABULOUSUI_API UFuSlateNavigationSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slate Navigation", Config)
	uint8 bApplySettings : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Navigation", Config)
	uint8 bAllowKeyNavigation : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Navigation",
		Config, Meta = (ForceInlineRow, EditCondition = "bAllowKeyNavigation"))
	TMap<FKey, EUINavigation> KeyNavigationMappings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation", Config)
	uint8 bAllowAnalogNavigation : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation",
		Meta = (ClampMin = 0, ClampMax = 1, EditCondition = "bAllowAnalogNavigation"))
	float AnalogNavigationThreshold{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation", Config, Meta = (EditCondition = "bAllowAnalogNavigation"))
	FKey AnalogNavigationVerticalKey{EKeys::Gamepad_LeftY};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation", Config, Meta = (EditCondition = "bAllowAnalogNavigation"))
	FKey AnalogNavigationHorizontalKey{EKeys::Gamepad_LeftX};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tab Navigation", Config)
	uint8 bAllowTabNavigation : 1 {true};

public:
	UFuSlateNavigationSettings();

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
};
