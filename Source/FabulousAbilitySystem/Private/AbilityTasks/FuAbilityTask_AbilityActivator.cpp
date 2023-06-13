#include "AbilityTasks/FuAbilityTask_AbilityActivator.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_AbilityActivator)

UFuAbilityTask_AbilityActivator* UFuAbilityTask_AbilityActivator::FuActivateAbilityByHandle(
	UGameplayAbility* OwningAbility, const FGameplayAbilitySpecHandle InAbilityHandle, const bool bInCancelAbilityOnDestroy)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->AbilityHandle = InAbilityHandle;
	Task->bCancelAbilityOnDestroy = bInCancelAbilityOnDestroy;

	return Task;
}

void UFuAbilityTask_AbilityActivator::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(AbilityHandle.IsValid()))
	{
		EndTask();
		return;
	}

	auto bAbilityEnded{false};
	auto bAbilityCanceled{false};

	const auto AbilityEndedHandle{
		AbilitySystemComponent->OnAbilityEnded.AddLambda([this, &bAbilityEnded, &bAbilityCanceled](const FAbilityEndedData& EndedData)
		{
			if (EndedData.AbilitySpecHandle == AbilityHandle)
			{
				bAbilityEnded = true;
				bAbilityCanceled = EndedData.bWasCancelled;
			}
		})
	};

	const auto bAbilityActivated{AbilitySystemComponent->TryActivateAbility(AbilityHandle)};

	AbilitySystemComponent->OnAbilityEnded.Remove(AbilityEndedHandle);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastAbilityTaskDelegates()};

	if (!bAbilityActivated)
	{
		if (bDelegatesBroadcastAllowed)
		{
			OnAbilityFailed.Broadcast();
		}

		EndTask();
		return;
	}

	if (bDelegatesBroadcastAllowed)
	{
		OnAbilityActivated.Broadcast();
	}

	if (bAbilityEnded)
	{
		if (bDelegatesBroadcastAllowed)
		{
			if (bAbilityCanceled)
			{
				OnAbilityCanceled.Broadcast();
			}
			else
			{
				OnAbilityEnded.Broadcast();
			}
		}

		EndTask();
		return;
	}

	AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityEnded);
}

void UFuAbilityTask_AbilityActivator::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->OnAbilityEnded.RemoveAll(this);

		if (bCancelAbilityOnDestroy && AbilityHandle.IsValid())
		{
			AbilitySystemComponent->CancelAbilityHandle(AbilityHandle);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_AbilityActivator::AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData)
{
	if (EndedData.AbilitySpecHandle != AbilityHandle)
	{
		return;
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (EndedData.bWasCancelled)
		{
			OnAbilityCanceled.Broadcast();
		}
		else
		{
			OnAbilityEnded.Broadcast();
		}
	}

	AbilityHandle = {};

	EndTask();
}
