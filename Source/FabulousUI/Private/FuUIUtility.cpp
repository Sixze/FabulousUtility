#include "FuUIUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuUIUtility)

UUserWidget* UFuUIUtility::CreateWidgetWithOwningWidget(const TSubclassOf<UUserWidget> UserWidgetClass, UWidget* OwningWidget)
{
	return CreateWidget(OwningWidget, UserWidgetClass);
}
