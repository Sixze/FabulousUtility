#include "FuEditorSettings.h"

#include "Framework/Application/SlateApplication.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEditorSettings)

#define LOCTEXT_NAMESPACE "FuEditorSettings"

UFuEditorSettings::UFuEditorSettings()
{
	CategoryName = FName{TEXTVIEW("Fabulous Utility")};
}

#if WITH_EDITOR
void UFuEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, bApplyEditorScale) ||
	    ChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, EditorScale))
	{
		FSlateApplication::Get().SetApplicationScale(bApplyEditorScale ? EditorScale : 1.0f);
	}

	Super::PostEditChangeProperty(ChangedEvent);
}

FText UFuEditorSettings::GetSectionText() const
{
	return LOCTEXT("Section", "Editor");
}
#endif

#undef LOCTEXT_NAMESPACE
