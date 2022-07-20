#pragma once

#include "Engine/DeveloperSettings.h"
#include "FuSlateNavigationSettings.generated.h"

UCLASS(Config = "Engine", DefaultConfig)
class FABULOUSUTILITY_API UFuSlateNavigationSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slate Navigation", Config)
	bool bDisableSlateNavigation;

public:
	UFuSlateNavigationSettings();

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
};
