#include "AbilitySystem/AbilityTasks/FuAbilityTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

UFuAbilityTask_ActivateAbility* UFuAbilityTask_ActivateAbility::FuActivateAbilityByHandle(
	UGameplayAbility* OwningAbility, const FGameplayAbilitySpecHandle AbilityHandle, const bool bCancelAbilityOnDestroy)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->AbilityHandle1 = AbilityHandle;
	Task->bCancelAbilityOnDestroy1 = bCancelAbilityOnDestroy;

	return Task;
}

void UFuAbilityTask_ActivateAbility::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(AbilityHandle1.IsValid()))
	{
		EndTask();
		return;
	}

	auto bAbilityEnded{false};
	auto bAbilityCanceled{false};

	const auto AbilityEndedHandle{
		AbilitySystemComponent->OnAbilityEnded.AddLambda([this, &bAbilityEnded, &bAbilityCanceled](const FAbilityEndedData& EndedData)
		{
			if (EndedData.AbilitySpecHandle == AbilityHandle1)
			{
				bAbilityEnded = true;
				bAbilityCanceled = EndedData.bWasCancelled;
			}
		})
	};

	const auto bAbilityActivated{AbilitySystemComponent->TryActivateAbility(AbilityHandle1)};

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

void UFuAbilityTask_ActivateAbility::OnDestroy(const bool bInOwnerFinished)
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->OnAbilityEnded.RemoveAll(this);

		if (bCancelAbilityOnDestroy1)
		{
			AbilitySystemComponent->CancelAbilityHandle(AbilityHandle1);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_ActivateAbility::AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData)
{
	if (EndedData.AbilitySpecHandle != AbilityHandle1)
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

	EndTask();
}
