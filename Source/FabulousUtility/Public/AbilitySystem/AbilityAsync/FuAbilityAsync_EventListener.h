#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_EventListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuEventReceivedDelegate, const FGameplayEventData&, Payload);

UCLASS(DisplayName = "Fu Event Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_EventListener : public UAbilityAsync
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Event Tag", Meta = (AllowPrivateAccess))
	FGameplayTag EventTag1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Event Listener Ability Async")
	FFuEventReceivedDelegate OnReceived;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EventListener* FuListenForEventActor(const AActor* Actor, FGameplayTag EventTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EventListener* FuListenForEvent(UAbilitySystemComponent* AbilitySystem, FGameplayTag EventTag);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

protected:
	virtual void OnEventReceived(const FGameplayEventData* Payload);
};
