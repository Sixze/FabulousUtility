#pragma once

#include "UITag.h"
#include "Engine/CancellableAsyncAction.h"
#include "Input/CommonInputMode.h"
#include "Input/UIActionBindingHandle.h"
#include "FuAsyncAction_UIActionListener.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuUIActionExecutedDelegate, const FUIActionTag&, ActionTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuUIActionHeldDelegate, const FUIActionTag&, ActionTag, float, HeldPercent);

USTRUCT(BlueprintType, DisplayName = "Fu UI Action Binding Arguments")
struct FABULOUSUTILITY_API FFuUIActionBindingArguments
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	ECommonInputMode InputMode{ECommonInputMode::Menu};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	TEnumAsByte<EInputEvent> KeyEvent{IE_Pressed};

	// A persistent binding is always registered and will be executed regardless of the activation
	// status of the binding widget's parentage. Persistent bindings also never stomp one another -
	// if two are bound to the same action, both will execute. Use should be kept to a minimum.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	bool bIsPersistent{false};

	// True to have this binding consume the triggering key input. Persistent bindings
	// that consume will prevent the key reaching non-persistent bindings and game agents.
	// Non-persistent bindings that consume will prevent the key reaching game agents.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	bool bConsumeInput{true};

	// Whether this binding can/should be displayed in an action bar (if one exists).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	bool bDisplayInActionBar{true};

	// Optional display name to associate with this binding instead of the default.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FText DisplayNameOverride;
};

UCLASS(DisplayName = "Fu UI Action Listener Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_UIActionListener : public UCancellableAsyncAction
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Widget", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UCommonUserWidget> Widget1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Action Arguments",
		Meta = (AllowPrivateAccess))
	FFuUIActionBindingArguments ActionArguments1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Action Tags", Meta = (AllowPrivateAccess))
	TArray<FUIActionTag> ActionTags1;

	TArray<FUIActionBindingHandle> ActionBindings;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu UI Action Listener Async Action")
	FFuUIActionExecutedDelegate OnActionExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu UI Action Listener Async Action")
	FFuUIActionHeldDelegate OnActionHeld;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Async Actions",
		DisplayName = "Fu Listen For UI Action", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Widget"))
	static UFuAsyncAction_UIActionListener* FuListenForUIAction(UCommonUserWidget* Widget, FUIActionTag ActionTag,
	                                                            FFuUIActionBindingArguments ActionArguments);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Async Actions",
		DisplayName = "Fu Listen For UI Actions", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Widget"))
	static UFuAsyncAction_UIActionListener* FuListenForUIActions(UCommonUserWidget* Widget, FGameplayTagContainer ActionTags,
	                                                             FFuUIActionBindingArguments ActionArguments);

public:
	virtual void Activate() override;

	virtual void Cancel() override;

	virtual bool ShouldBroadcastDelegates() const override;

private:
	void Widget_OnActionExecuted(FUIActionTag ActionTag) const;

	void Widget_OnActionHeld(float HeldPercent, FUIActionTag ActionTag) const;
};
