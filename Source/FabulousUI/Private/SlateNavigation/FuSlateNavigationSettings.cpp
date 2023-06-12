#include "SlateNavigation/FuSlateNavigationSettings.h"

#include "Types/SlateEnums.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuSlateNavigationSettings)

#define LOCTEXT_NAMESPACE "FuSlateNavigationSettings"

UFuSlateNavigationSettings::UFuSlateNavigationSettings()
{
	CategoryName = FName{TEXTVIEW("Fabulous Utility")};

	KeyNavigationMappings = {
		{EKeys::Up, EUINavigation::Up},
		{EKeys::Down, EUINavigation::Down},
		{EKeys::Left, EUINavigation::Left},
		{EKeys::Right, EUINavigation::Right},

		{EKeys::Gamepad_DPad_Up, EUINavigation::Up},
		{EKeys::Gamepad_DPad_Down, EUINavigation::Down},
		{EKeys::Gamepad_DPad_Left, EUINavigation::Left},
		{EKeys::Gamepad_DPad_Right, EUINavigation::Right}
	};
}

#if WITH_EDITOR
FText UFuSlateNavigationSettings::GetSectionText() const
{
	return LOCTEXT("Section", "Slate Navigation");
}
#endif

#undef LOCTEXT_NAMESPACE
