#include "FuCommonUIUtility.h"

#include "CommonInputSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuCommonUIUtility)

ECommonInputType UFuCommonUIUtility::GetInputType(const FKey& Key)
{
	// Based on FCommonInputPreprocessor::GetInputType().

	if (!Key.IsGamepadKey())
	{
		return ECommonInputType::MouseAndKeyboard;
	}

	if (!UCommonInputSubsystem::IsMobileGamepadKey(Key))
	{
		return ECommonInputType::Gamepad;
	}

	return ECommonInputType::Touch;
}

UUserWidget* UFuCommonUIUtility::FindRootUserWidgetByClass(const UWidget* Widget, const TSubclassOf<UUserWidget> UserWidgetClass)
{
	if (!IsValid(UserWidgetClass))
	{
		return nullptr;
	}

	UUserWidget* ResultUserWidget{nullptr};

	while (IsValid(Widget))
	{
		// The outer of every widget is the UWidgetTree it's in, and the outer of every UWidgetTree is the UUserWidget that owns it.

		const UWidgetTree* WidgetTree{Cast<UWidgetTree>(Widget->GetOuter())};
		if (!IsValid(WidgetTree))
		{
			break;
		}

		auto* UserWidget{Cast<UUserWidget>(WidgetTree->GetOuter())};
		if (IsValid(UserWidget) && UserWidget->IsA(UserWidgetClass))
		{
			ResultUserWidget = UserWidget;
		}

		Widget = UserWidget;
	}

	return ResultUserWidget;
}

UUserWidget* UFuCommonUIUtility::FindAncestorUserWidgetByClass(const UWidget* Widget,
                                                               const TSubclassOf<UUserWidget> AncestorUserWidgetClass)
{
	if (!IsValid(AncestorUserWidgetClass))
	{
		return nullptr;
	}

	while (IsValid(Widget))
	{
		// The outer of every widget is the UWidgetTree it's in, and the outer of every UWidgetTree is the UUserWidget that owns it.

		const UWidgetTree* WidgetTree{Cast<UWidgetTree>(Widget->GetOuter())};
		if (!IsValid(WidgetTree))
		{
			return nullptr;
		}

		auto* UserWidget{Cast<UUserWidget>(WidgetTree->GetOuter())};
		if (IsValid(UserWidget) && UserWidget->IsA(AncestorUserWidgetClass))
		{
			return UserWidget;
		}

		Widget = UserWidget;
	}

	return nullptr;
}
