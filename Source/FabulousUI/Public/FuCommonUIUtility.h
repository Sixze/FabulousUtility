#pragma once

#include "Blueprint/WidgetTree.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuCommonUIUtility.generated.h"

UCLASS(DisplayName = "Fu Common UI Utility")
class FABULOUSUI_API UFuCommonUIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous UI|Common UI Utility", Meta = (ReturnDisplayName = "Input Type"))
	static ECommonInputType GetInputType(const FKey& Key);

	template <typename UserWidgetType = UUserWidget> requires TIsDerivedFrom<UserWidgetType, UUserWidget>::Value
	static UserWidgetType* FindRootUserWidget(const UWidget* Widget);

	UFUNCTION(BlueprintPure, Category = "Fabulous UI|Common UI Utility", Meta = (DefaultToSelf = "Widget",
		UserWidgetClass = "/Script/UMG.UserWidget", DeterminesOutputType = "UserWidgetClass", ReturnDisplayName = "Root Widget"))
	static UUserWidget* FindRootUserWidgetByClass(const UWidget* Widget, TSubclassOf<UUserWidget> UserWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Common UI Utility", Meta = (DefaultToSelf = "Widget",
		UserWidgetClass = "/Script/UMG.UserWidget", DeterminesOutputType = "UserWidgetClass",
		DynamicOutputParam = "UserWidget", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryFindRootUserWidgetByClass(const UWidget* Widget, TSubclassOf<UUserWidget> UserWidgetClass, UUserWidget*& UserWidget);

	template <typename UserWidgetType = UUserWidget> requires TIsDerivedFrom<UserWidgetType, UUserWidget>::Value
	static UserWidgetType* FindParentUserWidget(const UWidget* Widget);

	UFUNCTION(BlueprintPure, Category = "Fabulous UI|Common UI Utility", Meta = (DefaultToSelf = "Widget",
		UserWidgetClass = "/Script/UMG.UserWidget", DeterminesOutputType = "UserWidgetClass", ReturnDisplayName = "Root Widget"))
	static UUserWidget* FindParentUserWidgetByClass(const UWidget* Widget, TSubclassOf<UUserWidget> UserWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Common UI Utility", Meta = (DefaultToSelf = "Widget",
		UserWidgetClass = "/Script/UMG.UserWidget", DeterminesOutputType = "UserWidgetClass",
		DynamicOutputParam = "UserWidget", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryFindParentUserWidgetByClass(const UWidget* Widget, TSubclassOf<UUserWidget> UserWidgetClass, UUserWidget*& UserWidget);
};

template <typename UserWidgetType> requires TIsDerivedFrom<UserWidgetType, UUserWidget>::Value
UserWidgetType* UFuCommonUIUtility::FindRootUserWidget(const UWidget* Widget)
{
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

inline bool UFuCommonUIUtility::TryFindRootUserWidgetByClass(const UWidget* Widget, const TSubclassOf<UUserWidget> UserWidgetClass,
                                                             UUserWidget*& UserWidget)
{
	UserWidget = FindRootUserWidgetByClass(Widget, UserWidgetClass);
	return IsValid(UserWidget);
}

template <typename UserWidgetType> requires TIsDerivedFrom<UserWidgetType, UUserWidget>::Value
UserWidgetType* UFuCommonUIUtility::FindParentUserWidget(const UWidget* Widget)
{
	while (IsValid(Widget))
	{
		// The outer of every widget is the UWidgetTree it's in, and the outer of every UWidgetTree is the UUserWidget that owns it.

		const auto* WidgetTree{Cast<UWidgetTree>(Widget->GetOuter())};
		if (!IsValid(WidgetTree))
		{
			return nullptr;
		}

		auto* UserWidget{Cast<UUserWidget>(WidgetTree->GetOuter())};
		if (IsValid(UserWidget) && UserWidget->IsA<UserWidgetType>())
		{
			return Cast<UserWidgetType>(UserWidget);
		}

		Widget = UserWidget;
	}

	return nullptr;
}

inline bool UFuCommonUIUtility::TryFindParentUserWidgetByClass(const UWidget* Widget, const TSubclassOf<UUserWidget> UserWidgetClass,
                                                               UUserWidget*& UserWidget)
{
	UserWidget = FindParentUserWidgetByClass(Widget, UserWidgetClass);
	return IsValid(UserWidget);
}
