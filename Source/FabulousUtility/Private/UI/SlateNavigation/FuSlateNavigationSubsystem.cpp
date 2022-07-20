#include "UI/SlateNavigation/FuSlateNavigationSubsystem.h"

#if !UE_SERVER

#include "Engine/GameInstance.h"
#include "Framework/Application/NavigationConfig.h"
#include "UI/SlateNavigation/FuSlateNavigationSettings.h"

void UFuSlateNavigationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// https://www.reddit.com/r/unrealengine/comments/bmibsu/how_to_disable_default_gamepad_behaviour_in/

	if (FSlateApplication::IsInitialized() && GetDefault<UFuSlateNavigationSettings>()->bDisableSlateNavigation)
	{
		FSlateApplication::Get().SetNavigationConfig(MakeShared<FNullNavigationConfig>());
	}
}

#endif
