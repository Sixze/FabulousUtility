#include "AbilityAsync/FuAbilityAsync_EventListener.h"

#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_EventListener)

UFuAbilityAsync_EventListener* UFuAbilityAsync_EventListener::ListenForEventOnActor(const AActor* Actor, const FGameplayTag InEventTag)
{
	return ListenForEvent(UFuAbilitySystemUtility::GetAbilitySystem(Actor), InEventTag);
}

UFuAbilityAsync_EventListener* UFuAbilityAsync_EventListener::ListenForEvent(UAbilitySystemComponent* AbilitySystem,
                                                                             const FGameplayTag InEventTag)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->EventTag = InEventTag;

	return Task;
}

void UFuAbilityAsync_EventListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || !FU_ENSURE(EventTag.IsValid()))
	{
		EndAction();
		return;
	}

	AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EventTag).AddUObject(this, &ThisClass::AbilitySystem_OnEventReceived);
}

void UFuAbilityAsync_EventListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EventTag).RemoveAll(this);
	}

	Super::EndAction();
}

void UFuAbilityAsync_EventListener::AbilitySystem_OnEventReceived(const FGameplayEventData* EventData)
{
	if (ShouldBroadcastDelegates())
	{
		OnEventReceived.Broadcast(*EventData);
	}
}
