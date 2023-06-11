#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_EventListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuEventListenerDelegate, const FGameplayEventData&, Payload);

UCLASS(DisplayName = "Fu Event Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_EventListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag EventTag;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Event Listener Ability Async")
	FFuEventListenerDelegate OnEventReceived;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EventListener* FuListenForEventOnActor(const AActor* Actor,
	                                                              UPARAM(DisplayName = "Event Tag") FGameplayTag InEventTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EventListener* FuListenForEvent(UAbilitySystemComponent* AbilitySystem,
	                                                       UPARAM(DisplayName = "Event Tag") FGameplayTag InEventTag);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

protected:
	virtual void AbilitySystem_OnEventReceived(const FGameplayEventData* Payload);
};
