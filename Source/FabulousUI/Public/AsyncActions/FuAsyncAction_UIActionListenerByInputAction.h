#pragma once

#include "FuUIActionBindingArguments.h"
#include "Engine/CancellableAsyncAction.h"
#include "Input/UIActionBindingHandle.h"
#include "Sound/SlateSound.h"
#include "FuAsyncAction_UIActionListenerByInputAction.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuUIActionListenerByInputActionDelegate, const UInputAction*, InputAction);

UCLASS(DisplayName = "Fu UI Action Listener by Input Action Async Action")
class FABULOUSUI_API UFuAsyncAction_UIActionListenerByInputAction : public UCancellableAsyncAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UCommonUserWidget> Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FFuUIActionBindingArguments ActionArguments;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (DisplayThumbnail = false))
	TArray<TObjectPtr<UInputAction>> InputActions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FSlateSound Sound;

	TArray<FUIActionBindingHandle> ActionHandles;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous UI|Async Actions|UI Action Listener by Input Action")
	FFuUIActionListenerByInputActionDelegate OnActionExecuted;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Async Actions",
		DisplayName = "Listen for UI Action by Input Action",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget"))
	static UFuAsyncAction_UIActionListenerByInputAction* ListenForUIActionByInputAction(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Input Action") UInputAction* InInputAction,
		UPARAM(DisplayName = "Sound") FSlateSound InSound,
		UPARAM(DisplayName = "Action Arguments") FFuUIActionBindingArguments InActionArguments);

	UFUNCTION(BlueprintCallable, Category = "Fabulous UI|Async Actions",
		DisplayName = "Listen for UI Actions by Input Actions",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget"))
	static UFuAsyncAction_UIActionListenerByInputAction* ListenForUIActionsByInputActions(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Input Actions") TArray<UInputAction*> InInputActions,
		UPARAM(DisplayName = "Sound") FSlateSound InSound,
		UPARAM(DisplayName = "Action Arguments") FFuUIActionBindingArguments InActionArguments);

public:
	virtual void Activate() override;

	virtual void SetReadyToDestroy() override;

	virtual bool ShouldBroadcastDelegates() const override;

private:
	void Widget_OnActionExecuted(TWeakObjectPtr<UInputAction> InputAction) const;
};
