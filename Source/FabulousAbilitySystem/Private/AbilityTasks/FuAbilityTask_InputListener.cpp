#include "AbilityTasks/FuAbilityTask_InputListener.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_InputListener)

UFuAbilityTask_InputListener* UFuAbilityTask_InputListener::FuWaitForInput(UGameplayAbility* OwningAbility,
                                                                           const bool bInCheckInitialInputState)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->bCheckInitialInputState = bInCheckInitialInputState;

	return Task;
}

void UFuAbilityTask_InputListener::Activate()
{
	Super::Activate();

	AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed,
	                                                       GetAbilitySpecHandle(), GetActivationPredictionKey())
	                      .AddUObject(this, &ThisClass::AbilitySystem_OnInputPressed);

	AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased,
	                                                       GetAbilitySpecHandle(), GetActivationPredictionKey())
	                      .AddUObject(this, &ThisClass::AbilitySystem_OnInputReleased);

	if (bCheckInitialInputState && IsLocallyControlled())
	{
		if (Ability->GetCurrentAbilitySpec()->InputPressed)
		{
			AbilitySystem_OnInputPressed();
		}
		else
		{
			AbilitySystem_OnInputReleased();
		}
	}

	if (IsForRemoteClient())
	{
		const auto bInputPressCalled{
			AbilitySystemComponent->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputPressed,
			                                                         GetAbilitySpecHandle(), GetActivationPredictionKey())
		};

		const auto bInputReleaseCalled{
			AbilitySystemComponent->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputReleased,
			                                                         GetAbilitySpecHandle(), GetActivationPredictionKey())
		};

		if (!bInputPressCalled && !bInputReleaseCalled)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UFuAbilityTask_InputListener::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed,
		                                                       GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);

		AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased,
		                                                       GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputListener::AbilitySystem_OnInputPressed()
{
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get(), IsPredictingClient());

	if (IsPredictingClient())
	{
		AbilitySystemComponent->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
		                                                 GetAbilitySpecHandle(), GetActivationPredictionKey(),
		                                                 AbilitySystemComponent->ScopedPredictionKey);
	}
	else
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
		                                                      GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnInputPressed.Broadcast();
	}

	if (IsForRemoteClient())
	{
		SetWaitingOnRemotePlayerData();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputListener::AbilitySystem_OnInputReleased()
{
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get(), IsPredictingClient());

	if (IsPredictingClient())
	{
		AbilitySystemComponent->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
		                                                 GetAbilitySpecHandle(), GetActivationPredictionKey(),
		                                                 AbilitySystemComponent->ScopedPredictionKey);
	}
	else
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
		                                                      GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnInputReleased.Broadcast();
	}

	if (IsForRemoteClient())
	{
		SetWaitingOnRemotePlayerData();
	}
}
