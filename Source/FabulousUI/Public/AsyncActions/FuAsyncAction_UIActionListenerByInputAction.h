#pragma once

#include "CommonInputModeTypes.h"
#include "Engine/CancellableAsyncAction.h"
#include "Engine/EngineBaseTypes.h"
#include "Input/UIActionBindingHandle.h"
#include "FuAsyncAction_UIActionListenerByInputAction.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuUIActionListenerByInputActionDelegate, const UInputAction*, InputAction);

UCLASS(DisplayName = "Fu UI Action Listener By Input Action Async Action")
class FABULOUSUI_API UFuAsyncAction_UIActionListenerByInputAction : public UCancellableAsyncAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UCommonUserWidget> Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	ECommonInputMode InputMode{ECommonInputMode::Menu};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TEnumAsByte<EInputEvent> KeyEvent{IE_Pressed};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bPersistent : 1 {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bConsumeInput : 1 {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bDisplayInActionBar : 1 {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FText DisplayNameOverride;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (DisplayThumbnail = false))
	TArray<TObjectPtr<UInputAction>> InputActions;

	TArray<FUIActionBindingHandle> ActionHandles;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|UI Action Listener By Input Action Async Action")
	FFuUIActionListenerByInputActionDelegate OnActionExecuted;

public:
	/**
	 * @param bInPersistent A persistent binding is always registered and will be executed regardless of
	 * the activation status of the binding widget's parentage. Persistent bindings also never stomp one
	 * another - if two are bound to the same action, both will execute. Use should be kept to a minimum.
	 * @param bInConsumeInput True to have this binding consume the triggering key input. Persistent
	 * bindings that consume will prevent the key reaching non-persistent bindings and game
	 * agents. Non-persistent bindings that consume will prevent the key reaching game agents.
	 * @param bInDisplayInActionBar Whether this binding can/should be displayed in an action bar (if one exists).
	 * @param InDisplayNameOverride Optional display name to associate with this binding instead of the default.
	*/
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|UI Async Actions",
		DisplayName = "Listen For UI Action By Input Action", BlueprintInternalUseOnly,
		Meta = (DefaultToSelf = "InWidget", AdvancedDisplay = 2))
	static UFuAsyncAction_UIActionListenerByInputAction* ListenForUIActionByInputAction(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Input Action") UInputAction* InInputAction,
		UPARAM(DisplayName = "Input Mode") ECommonInputMode InInputMode = ECommonInputMode::Menu,
		UPARAM(DisplayName = "Key Event") TEnumAsByte<EInputEvent> InKeyEvent = IE_Pressed,
		UPARAM(DisplayName = "Persistent") bool bInPersistent = false,
		UPARAM(DisplayName = "Consume Input") bool bInConsumeInput = true,
		UPARAM(DisplayName = "Display in Action Bar") bool bInDisplayInActionBar = true,
		UPARAM(DisplayName = "Display Name Override") FText InDisplayNameOverride = FText());

	/**
	 * @param bInPersistent A persistent binding is always registered and will be executed regardless of
	 * the activation status of the binding widget's parentage. Persistent bindings also never stomp one
	 * another - if two are bound to the same action, both will execute. Use should be kept to a minimum.
	 * @param bInConsumeInput True to have this binding consume the triggering key input. Persistent
	 * bindings that consume will prevent the key reaching non-persistent bindings and game
	 * agents. Non-persistent bindings that consume will prevent the key reaching game agents.
	 * @param bInDisplayInActionBar Whether this binding can/should be displayed in an action bar (if one exists).
	 * @param InDisplayNameOverride Optional display name to associate with this binding instead of the default.
	*/
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|UI Async Actions",
		DisplayName = "Listen For UI Actions By Input Actions", BlueprintInternalUseOnly,
		Meta = (DefaultToSelf = "InWidget", AdvancedDisplay = 2))
	static UFuAsyncAction_UIActionListenerByInputAction* ListenForUIActionsByInputActions(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Input Actions") TArray<UInputAction*> InInputActions,
		UPARAM(DisplayName = "Input Mode") ECommonInputMode InInputMode = ECommonInputMode::Menu,
		UPARAM(DisplayName = "Key Event") TEnumAsByte<EInputEvent> InKeyEvent = IE_Pressed,
		UPARAM(DisplayName = "Persistent") bool bInPersistent = false,
		UPARAM(DisplayName = "Consume Input") bool bInConsumeInput = true,
		UPARAM(DisplayName = "Display in Action Bar") bool bInDisplayInActionBar = true,
		UPARAM(DisplayName = "Display Name Override") FText InDisplayNameOverride = FText());

public:
	virtual void Activate() override;

	virtual void Cancel() override;

	virtual bool ShouldBroadcastDelegates() const override;

private:
	void Widget_OnActionExecuted(TWeakObjectPtr<UInputAction> InputAction) const;
};
