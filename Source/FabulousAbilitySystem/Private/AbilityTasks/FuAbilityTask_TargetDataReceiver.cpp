#include "AbilityTasks/FuAbilityTask_TargetDataReceiver.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_TargetDataReceiver)

UFuAbilityTask_TargetDataReceiver* UFuAbilityTask_TargetDataReceiver::FuReceiveTargetData(UGameplayAbility* OwningAbility,
                                                                                          const bool bInReceiveOnce)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->bReceiveOnce = bInReceiveOnce;

	return Task;
}

void UFuAbilityTask_TargetDataReceiver::Activate()
{
	Super::Activate();

	if (!Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		EndTask();
		return;
	}

	AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey())
	                      .AddUObject(this, &ThisClass::AbilitySystem_OnAbilityTargetDataSet);

	AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());

	SetWaitingOnRemotePlayerData();
}

void UFuAbilityTask_TargetDataReceiver::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_TargetDataReceiver::AbilitySystem_OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData,
                                                                             const FGameplayTag ActivationTag)
{
	const auto TargetDataCopy{TargetData};

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReceived.Broadcast(TargetDataCopy);
	}

	if (bReceiveOnce)
	{
		EndTask();
	}
	else
	{
		SetWaitingOnRemotePlayerData();
	}
}
