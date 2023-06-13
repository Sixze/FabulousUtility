#include "SlateNavigation/FuSlateNavigationSubsystem.h"

#if !UE_SERVER

#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "SlateNavigation/FuSlateNavigationSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuSlateNavigationSubsystem)

bool UFuSlateNavigationSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return !CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance() && Super::ShouldCreateSubsystem(Outer);
}

void UFuSlateNavigationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const auto* Settings{GetDefault<UFuSlateNavigationSettings>()};

	if (FSlateApplication::IsInitialized() && Settings->bApplySettings)
	{
		const auto NavigationConfig{MakeShared<FNavigationConfig>()};

		NavigationConfig->bTabNavigation = Settings->bAllowTabNavigation;
		NavigationConfig->bKeyNavigation = Settings->bAllowKeyNavigation;
		NavigationConfig->bAnalogNavigation = Settings->bAllowAnalogNavigation;

		NavigationConfig->AnalogNavigationHorizontalThreshold = Settings->AnalogNavigationThreshold;
		NavigationConfig->AnalogNavigationVerticalThreshold = Settings->AnalogNavigationThreshold;

		NavigationConfig->AnalogHorizontalKey = Settings->AnalogNavigationHorizontalKey;
		NavigationConfig->AnalogVerticalKey = Settings->AnalogNavigationVerticalKey;

		NavigationConfig->KeyEventRules = Settings->KeyNavigationMappings;

		FSlateApplication::Get().SetNavigationConfig(NavigationConfig);
	}
}

void UFuSlateNavigationSubsystem::Deinitialize()
{
#if WITH_EDITOR
	if (FSlateApplication::IsInitialized() && GetDefault<UFuSlateNavigationSettings>()->bApplySettings)
	{
		FSlateApplication::Get().SetNavigationConfig(MakeShared<FNavigationConfig>());
	}
#endif

	Super::Deinitialize();
}

#endif
