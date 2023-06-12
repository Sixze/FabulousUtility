#pragma once

#include "CommonInputModeTypes.h"
#include "UITag.h"
#include "Engine/CancellableAsyncAction.h"
#include "Engine/EngineBaseTypes.h"
#include "Input/UIActionBindingHandle.h"
#include "FuAsyncAction_UIActionListener.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuUIActionListenerDelegate, const FGameplayTag&, ActionTag, float, HeldPercent);

UCLASS(DisplayName = "Fu UI Action Listener Async Action")
class FABULOUSUI_API UFuAsyncAction_UIActionListener : public UCancellableAsyncAction
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
	bool bPersistent{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bConsumeInput{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bDisplayInActionBar{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FText DisplayNameOverride;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TArray<FUIActionTag> ActionTags;

	TArray<FUIActionBindingHandle> ActionHandles;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu UI Action Listener Async Action")
	FFuUIActionListenerDelegate OnActionExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu UI Action Listener Async Action")
	FFuUIActionListenerDelegate OnActionHeld;

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
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu UI Async Actions",
		DisplayName = "Fu Listen For UI Action", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget", AdvancedDisplay = 2))
	static UFuAsyncAction_UIActionListener* FuListenForUIAction(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Action Tag") FUIActionTag InActionTag,
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
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu UI Async Actions",
		DisplayName = "Fu Listen For UI Actions", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget", AdvancedDisplay = 2))
	static UFuAsyncAction_UIActionListener* FuListenForUIActions(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Action Tags") FGameplayTagContainer InActionTags,
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
	void Widget_OnActionExecuted(FUIActionTag ActionTag) const;

	void Widget_OnActionHeld(float HeldPercent, FUIActionTag ActionTag) const;
};
