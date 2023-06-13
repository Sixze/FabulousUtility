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
	bool bApplySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Navigation", Config)
	bool bAllowKeyNavigation{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Navigation", Config,
		Meta = (ForceInlineRow, EditCondition = "bAllowKeyNavigation"))
	TMap<FKey, EUINavigation> KeyNavigationMappings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation", Config)
	bool bAllowAnalogNavigation{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation",
		Meta = (ClampMin = 0, ClampMax = 1, EditCondition = "bAllowAnalogNavigation"))
	float AnalogNavigationThreshold{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation",
		Config, Meta = (EditCondition = "bAllowAnalogNavigation"))
	FKey AnalogNavigationVerticalKey{EKeys::Gamepad_LeftY};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Analog Navigation",
		Config, Meta = (EditCondition = "bAllowAnalogNavigation"))
	FKey AnalogNavigationHorizontalKey{EKeys::Gamepad_LeftX};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tab Navigation", Config)
	bool bAllowTabNavigation{true};

public:
	UFuSlateNavigationSettings();

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
};
