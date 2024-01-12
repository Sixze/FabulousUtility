#pragma once

#include "Blueprint/UserWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuUIUtility.generated.h"

UCLASS(DisplayName = "Fu UI Utility")
class FABULOUSUI_API UFuUIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Unlike the Create Widget node, this function exposes the Owning Widget parameter instead of Owning Player.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|UI Utility",
		Meta = (DefaultToSelf = "OwningWidget", DeterminesOutputType = "UserWidgetClass", ReturnDisplayName = "User Widget"))
	static UUserWidget* CreateWidgetWithOwningWidget(TSubclassOf<UUserWidget> UserWidgetClass, UWidget* OwningWidget);
};
