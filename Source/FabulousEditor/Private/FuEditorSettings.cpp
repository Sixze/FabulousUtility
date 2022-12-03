#include "FuEditorSettings.h"

#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "FuEditorSettings"

UFuEditorSettings::UFuEditorSettings()
{
	CategoryName = TEXT("Fabulous Utility");
}

#if WITH_EDITOR
void UFuEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, bApplyEditorScale) ||
	    PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, EditorScale))
	{
		FSlateApplication::Get().SetApplicationScale(bApplyEditorScale ? EditorScale : 1.0f);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

FText UFuEditorSettings::GetSectionText() const
{
	return LOCTEXT("Section", "Editor");
}
#endif

#undef LOCTEXT_NAMESPACE
