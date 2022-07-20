#include "UI/SlateNavigation/FuSlateNavigationSettings.h"

#define LOCTEXT_NAMESPACE "SlateNavigationSettings"

UFuSlateNavigationSettings::UFuSlateNavigationSettings()
{
	CategoryName = TEXT("Fabulous Utility");
}

#if WITH_EDITOR
FText UFuSlateNavigationSettings::GetSectionText() const
{
	return LOCTEXT("Section", "Slate Navigation");
}
#endif

#undef LOCTEXT_NAMESPACE
