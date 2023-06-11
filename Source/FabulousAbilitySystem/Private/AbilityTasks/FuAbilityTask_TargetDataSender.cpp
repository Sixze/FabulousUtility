#include "AbilityTasks/FuAbilityTask_TargetDataSender.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_TargetDataSender)

UFuAbilityTask_TargetDataSender* UFuAbilityTask_TargetDataSender::FuSendTargetData(UGameplayAbility* OwningAbility,
                                                                                   const FGameplayAbilityTargetDataHandle& InTargetData)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->TargetData = InTargetData;

	return Task;
}

void UFuAbilityTask_TargetDataSender::Activate()
{
	Super::Activate();

	if (!IsLocallyControlled())
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey())
		                      .AddUObject(this, &ThisClass::AbilitySystem_OnAbilityTargetDataSet);

		AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());

		SetWaitingOnRemotePlayerData();
		return;
	}

	FScopedPredictionWindow PredictionWindow{
		AbilitySystemComponent.Get(),
		!Ability->GetCurrentActorInfo()->IsNetAuthority() && !AbilitySystemComponent->ScopedPredictionKey.IsValidForMorePrediction()
	};

	if (IsPredictingClient())
	{
		AbilitySystemComponent->CallServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), TargetData,
		                                                          {}, AbilitySystemComponent->ScopedPredictionKey);
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReceived.Broadcast(TargetData);
	}

	EndTask();
}

void UFuAbilityTask_TargetDataSender::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_TargetDataSender::AbilitySystem_OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& NewTargetData,
                                                                           FGameplayTag ActivationTag)
{
	const auto TargetDataCopy{NewTargetData};

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReceived.Broadcast(TargetDataCopy);
	}

	EndTask();
}
