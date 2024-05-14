#pragma once

#include "FuUIActionBindingArguments.h"
#include "UITag.h"
#include "Engine/CancellableAsyncAction.h"
#include "Input/UIActionBindingHandle.h"
#include "Sound/SlateSound.h"
#include "FuAsyncAction_UIActionListenerByTag.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuUIActionListenerByTagDelegate, const FGameplayTag&, ActionTag, float, HeldPercent);

UCLASS(DisplayName = "Fu UI Action Listener by Tag Async Action")
class FABULOUSUI_API UFuAsyncAction_UIActionListenerByTag : public UCancellableAsyncAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UCommonUserWidget> Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FFuUIActionBindingArguments ActionArguments;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TArray<FUIActionTag> ActionTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FSlateSound Sound;

	TArray<FUIActionBindingHandle> ActionHandles;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous UI|Async Actions|UI Action Listener by Tag")
	FFuUIActionListenerByTagDelegate OnActionExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous UI|Async Actions|UI Action Listener by Tag")
	FFuUIActionListenerByTagDelegate OnActionHeld;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Async Actions",
		DisplayName = "Listen for UI Action by Tag",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget"))
	static UFuAsyncAction_UIActionListenerByTag* ListenForUIActionByTag(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Action Tag") FUIActionTag InActionTag,
		UPARAM(DisplayName = "Sound") FSlateSound InSound,
		UPARAM(DisplayName = "Action Arguments") FFuUIActionBindingArguments InActionArguments);

	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Async Actions",
		DisplayName = "Listen for UI Actions by Tags",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget"))
	static UFuAsyncAction_UIActionListenerByTag* ListenForUIActionsByTags(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Action Tags", Meta = (Categories = "UI.Action")) FGameplayTagContainer InActionTags,
		UPARAM(DisplayName = "Sound") FSlateSound InSound,
		UPARAM(DisplayName = "Action Arguments") FFuUIActionBindingArguments InActionArguments);

public:
	virtual void Activate() override;

	virtual void SetReadyToDestroy() override;

	virtual bool ShouldBroadcastDelegates() const override;

private:
	void Widget_OnActionExecuted(FUIActionTag ActionTag) const;

	void Widget_OnActionHeld(float HeldPercent, FUIActionTag ActionTag) const;
};
