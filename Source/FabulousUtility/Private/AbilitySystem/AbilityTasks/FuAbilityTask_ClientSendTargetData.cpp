#include "AbilitySystem/AbilityTasks/FuAbilityTask_ClientSendTargetData.h"

#include "AbilitySystemComponent.h"

UFuAbilityTask_ClientSendTargetData* UFuAbilityTask_ClientSendTargetData::FuClientSendTargetData(
	UGameplayAbility* OwningAbility, const FGameplayAbilityTargetDataHandle& TargetData)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->TargetData1 = TargetData;

	return Task;
}

void UFuAbilityTask_ClientSendTargetData::Activate()
{
	Super::Activate();

	if (IsLocallyControlled())
	{
		// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
		FScopedPredictionWindow ScopedPredictionWindow{
			AbilitySystemComponent,
			!Ability->GetCurrentActorInfo()->IsNetAuthority() && !AbilitySystemComponent->ScopedPredictionKey.IsValidForMorePrediction()
		};

		if (IsPredictingClient())
		{
			AbilitySystemComponent->CallServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), TargetData1,
			                                                          {}, AbilitySystemComponent->ScopedPredictionKey);
		}

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnTargetDataReceived.Broadcast(TargetData1);
		}

		EndTask();
	}
	else
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey())
		                      .AddUObject(this, &ThisClass::OnAbilityTargetDataSet);

		AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());

		SetWaitingOnRemotePlayerData();
	}
}

void UFuAbilityTask_ClientSendTargetData::OnDestroy(const bool bInOwnerFinished)
{
	AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_ClientSendTargetData::OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData,
                                                                 FGameplayTag ActivationTag)
{
	const auto TargetDataCopy{TargetData};

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReceived.Broadcast(TargetDataCopy);
	}

	EndTask();
}
