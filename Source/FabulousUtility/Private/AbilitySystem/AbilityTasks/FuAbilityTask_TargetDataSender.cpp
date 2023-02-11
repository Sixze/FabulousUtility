#include "AbilitySystem/AbilityTasks/FuAbilityTask_TargetDataSender.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_TargetDataSender)

UFuAbilityTask_TargetDataSender* UFuAbilityTask_TargetDataSender::FuSendTargetData(UGameplayAbility* OwningAbility,
                                                                                   const FGameplayAbilityTargetDataHandle& TargetData)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->TargetData1 = TargetData;

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

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
	FScopedPredictionWindow PredictionWindow{
		AbilitySystemComponent.Get(),
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

void UFuAbilityTask_TargetDataSender::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_TargetDataSender::AbilitySystem_OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData,
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
