#pragma once

#include "Blueprint/UserWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuUIUtility.generated.h"

UCLASS(DisplayName = "Fu UI Utility")
class FABULOUSUI_API UFuUIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Unlike the Create Widget node, this function exposes the OwningWidget parameter instead of OwningPlayer.
	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Utility",
		Meta = (DefaultToSelf = "OwningWidget", DeterminesOutputType = "UserWidgetClass", ReturnDisplayName = "User Widget"))
	static UUserWidget* CreateWidgetWithOwningWidget(TSubclassOf<UUserWidget> UserWidgetClass, UWidget* OwningWidget);

	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Utility")
	static void InvalidatePaint(UWidget* Widget);
};
