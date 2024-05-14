#pragma once

#include "CommonInputModeTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "FuUIActionBindingArguments.generated.h"

struct FBindUIActionArgs;

USTRUCT(BlueprintType, DisplayName = "Fu UI Action Binding Arguments")
struct FABULOUSUI_API FFuUIActionBindingArguments
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	ECommonInputMode InputMode{ECommonInputMode::Menu};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	TEnumAsByte<EInputEvent> KeyEvent{IE_Pressed};

	// A persistent binding is always registered and will be executed regardless of the activation
	// status of the binding widget's parentage. Persistent bindings also never stomp one another
	// - if two are bound to the same action, both will execute. Use should be kept to a minimum.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	uint8 bPersistent : 1 {false};

	// True to have this binding consume the triggering key input. Persistent bindings
	// that consume will prevent the key reaching non-persistent bindings and game agents.
	// Non-persistent bindings that consume will prevent the key reaching game agents.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	uint8 bConsumeInput : 1 {true};

	// Whether this binding can/should be displayed in an action bar (if one exists).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	uint8 bDisplayInActionBar : 1 {true};

	// Optional display name to associate with this binding instead of the default.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous UI")
	FText DisplayNameOverride;

public:
	void Fill(FBindUIActionArgs& ActionArguments) const;
};
