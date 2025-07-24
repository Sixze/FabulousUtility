#include "FuUIUtility.h"

#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuUIUtility)

UUserWidget* UFuUIUtility::CreateWidgetWithOwningWidget(const TSubclassOf<UUserWidget> UserWidgetClass, UWidget* OwningWidget)
{
	return CreateWidget(OwningWidget, UserWidgetClass);
}

void UFuUIUtility::InvalidatePaint(UWidget* Widget)
{
	auto* SlateWidget{FU_ENSURE(IsValid(Widget)) ? Widget->GetCachedWidget().Get() : nullptr};
	if (SlateWidget != nullptr)
	{
		SlateWidget->Invalidate(EInvalidateWidgetReason::Paint);
	}
}
