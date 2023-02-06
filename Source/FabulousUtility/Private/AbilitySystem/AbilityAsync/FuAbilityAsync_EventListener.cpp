#include "AbilitySystem/AbilityAsync/FuAbilityAsync_EventListener.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FuMacros.h"

UFuAbilityAsync_EventListener* UFuAbilityAsync_EventListener::FuListenForEventOnActor(const AActor* Actor, const FGameplayTag EventTag)
{
	return FuListenForEvent(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), EventTag);
}

UFuAbilityAsync_EventListener* UFuAbilityAsync_EventListener::FuListenForEvent(UAbilitySystemComponent* AbilitySystem,
                                                                               const FGameplayTag EventTag)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->EventTag1 = EventTag;

	return Task;
}

void UFuAbilityAsync_EventListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || !FU_ENSURE(EventTag1.IsValid()))
	{
		EndAction();
		return;
	}

	AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EventTag1).AddUObject(this, &ThisClass::AbilitySystem_OnEventReceived);
}

void UFuAbilityAsync_EventListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EventTag1).RemoveAll(this);
	}

	Super::EndAction();
}

void UFuAbilityAsync_EventListener::AbilitySystem_OnEventReceived(const FGameplayEventData* Payload)
{
	if (ShouldBroadcastDelegates())
	{
		OnEventReceived.Broadcast(*Payload);
	}
}
