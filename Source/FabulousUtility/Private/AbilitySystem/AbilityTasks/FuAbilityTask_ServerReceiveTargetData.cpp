#include "AbilitySystem/AbilityTasks/FuAbilityTask_ServerReceiveTargetData.h"

#include "AbilitySystemComponent.h"

UFuAbilityTask_ServerReceiveTargetData* UFuAbilityTask_ServerReceiveTargetData::FuServerWaitForTargetData(
	UGameplayAbility* OwningAbility, const bool bReceiveOnce)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->bReceiveOnce1 = bReceiveOnce;

	return Task;
}

void UFuAbilityTask_ServerReceiveTargetData::Activate()
{
	Super::Activate();

	if (Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey())
		                      .AddUObject(this, &ThisClass::OnAbilityTargetDataSet);

		AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());

		SetWaitingOnRemotePlayerData();
	}
}

void UFuAbilityTask_ServerReceiveTargetData::OnDestroy(const bool bInOwnerFinished)
{
	AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_ServerReceiveTargetData::OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData,
                                                                    const FGameplayTag ActivationTag)
{
	const auto TargetDataCopy{TargetData};

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReceived.Broadcast(TargetDataCopy);
	}

	if (bReceiveOnce1)
	{
		EndTask();
	}
}
