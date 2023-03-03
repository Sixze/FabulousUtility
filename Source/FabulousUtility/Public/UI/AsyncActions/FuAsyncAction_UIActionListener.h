#pragma once

#include "UITag.h"
#include "CommonInputModeTypes.h"
#include "Engine/CancellableAsyncAction.h"
#include "Input/UIActionBindingHandle.h"
#include "FuAsyncAction_UIActionListener.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuUIActionListenerDelegate, const FGameplayTag&, ActionTag, float, HeldPercent);

UCLASS(DisplayName = "Fu UI Action Listener Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_UIActionListener : public UCancellableAsyncAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Widget")
	TWeakObjectPtr<UCommonUserWidget> Widget1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Input Mode")
	ECommonInputMode InputMode1{ECommonInputMode::Menu};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Key Event")
	TEnumAsByte<EInputEvent> KeyEvent1{IE_Pressed};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Persistent")
	bool bPersistent1{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Consume Input")
	bool bConsumeInput1{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Display in Action Bar")
	bool bDisplayInActionBar1{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Display Name Override")
	FText DisplayNameOverride1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Action Tags")
	TArray<FUIActionTag> ActionTags1;

	TArray<FUIActionBindingHandle> ActionHandles;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu UI Action Listener Async Action")
	FFuUIActionListenerDelegate OnActionExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu UI Action Listener Async Action")
	FFuUIActionListenerDelegate OnActionHeld;

public:
	/**
	 * @param bPersistent A persistent binding is always registered and will be executed regardless of
	 * the activation status of the binding widget's parentage. Persistent bindings also never stomp one
	 * another - if two are bound to the same action, both will execute. Use should be kept to a minimum.
	 * @param bConsumeInput True to have this binding consume the triggering key input. Persistent
	 * bindings that consume will prevent the key reaching non-persistent bindings and game
	 * agents. Non-persistent bindings that consume will prevent the key reaching game agents.
	 * @param bDisplayInActionBar Whether this binding can/should be displayed in an action bar (if one exists).
	 * @param DisplayNameOverride Optional display name to associate with this binding instead of the default.
	*/
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu UI Async Actions",
		DisplayName = "Fu Listen For UI Action", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Widget", AdvancedDisplay = 2))
	static UFuAsyncAction_UIActionListener* FuListenForUIAction(UCommonUserWidget* Widget, FUIActionTag ActionTag,
	                                                            ECommonInputMode InputMode = ECommonInputMode::Menu,
	                                                            TEnumAsByte<EInputEvent> KeyEvent = IE_Pressed,
	                                                            bool bPersistent = false, bool bConsumeInput = true,
	                                                            bool bDisplayInActionBar = true, FText DisplayNameOverride = FText());

	/**
	 * @param bPersistent A persistent binding is always registered and will be executed regardless of
	 * the activation status of the binding widget's parentage. Persistent bindings also never stomp one
	 * another - if two are bound to the same action, both will execute. Use should be kept to a minimum.
	 * @param bConsumeInput True to have this binding consume the triggering key input. Persistent
	 * bindings that consume will prevent the key reaching non-persistent bindings and game
	 * agents. Non-persistent bindings that consume will prevent the key reaching game agents.
	 * @param bDisplayInActionBar Whether this binding can/should be displayed in an action bar (if one exists).
	 * @param DisplayNameOverride Optional display name to associate with this binding instead of the default.
	*/
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu UI Async Actions",
		DisplayName = "Fu Listen For UI Actions", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Widget", AdvancedDisplay = 2))
	static UFuAsyncAction_UIActionListener* FuListenForUIActions(UCommonUserWidget* Widget, FGameplayTagContainer ActionTags,
	                                                             ECommonInputMode InputMode = ECommonInputMode::Menu,
	                                                             TEnumAsByte<EInputEvent> KeyEvent = IE_Pressed,
	                                                             bool bPersistent = false, bool bConsumeInput = true,
	                                                             bool bDisplayInActionBar = true, FText DisplayNameOverride = FText());

public:
	virtual void Activate() override;

	virtual void Cancel() override;

	virtual bool ShouldBroadcastDelegates() const override;

private:
	void Widget_OnActionExecuted(FUIActionTag ActionTag) const;

	void Widget_OnActionHeld(float HeldPercent, FUIActionTag ActionTag) const;
};
