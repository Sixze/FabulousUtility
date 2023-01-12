#pragma once

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuCommonUIUtility.generated.h"

enum class ECommonInputType : uint8;

UCLASS(DisplayName = "Fu Common UI Utility")
class FABULOUSUTILITY_API UFuCommonUIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Common UI Utility")
	static ECommonInputType GetInputTypeForKey(const FKey& Key);

	template <typename UserWidgetType = UUserWidget>
	static UserWidgetType* FindRootUserWidget(const UWidget* Widget);
};

template <typename UserWidgetType>
UserWidgetType* UFuCommonUIUtility::FindRootUserWidget(const UWidget* Widget)
{
	static_assert(TIsDerivedFrom<UserWidgetType, UUserWidget>::IsDerived);

	UserWidgetType* ResultUserWidget{nullptr};

	while (IsValid(Widget))
	{
		// The outer of every widget is the UWidgetTree it's in, and the outer of every UWidgetTree is the UUserWidget that owns it.

		const auto* WidgetTree{Cast<UWidgetTree>(Widget->GetOuter())};
		if (!IsValid(WidgetTree))
		{
			break;
		}

		auto* UserWidget{Cast<UUserWidget>(WidgetTree->GetOuter())};
		if (IsValid(UserWidget) && UserWidget->IsA<UserWidgetType>())
		{
			ResultUserWidget = Cast<UserWidgetType>(UserWidget);
		}

		Widget = UserWidget;
	}

	return ResultUserWidget;
}
