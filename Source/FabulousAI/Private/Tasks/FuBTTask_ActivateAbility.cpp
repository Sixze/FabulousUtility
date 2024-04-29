#include "Tasks/FuBTTask_ActivateAbility.h"

#include "AIController.h"
#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTTask_ActivateAbility)

struct FFuActivateAbilityMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;

	TSet<FGameplayAbilitySpecHandle> ActiveAbilityHandles;

	uint8 bAnyAbilitySuccessfullyEnded : 1 {false};
};

UFuBTTask_ActivateAbility::UFuBTTask_ActivateAbility()
{
	NodeName = TEXTVIEW("Fu Activate Ability");
	bIgnoreRestartSelf = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

uint16 UFuBTTask_ActivateAbility::GetInstanceMemorySize() const
{
	return sizeof(FFuActivateAbilityMemory);
}

FString UFuBTTask_ActivateAbility::GetStaticDescription() const
{
	TStringBuilder<256> DescriptionBuilder;

	if (bWaitForAbilityEnd)
	{
		DescriptionBuilder << (bCancelAbilityOnAbort
			                       ? TEXTVIEW("( wait for end, cancel on abort )") LINE_TERMINATOR
			                       : TEXTVIEW("( wait for end )") LINE_TERMINATOR);
	}

	DescriptionBuilder << TEXTVIEW("Activate Ability:");

	if (AbilityTag.IsValid())
	{
		DescriptionBuilder << AbilityTag.GetTagName();
	}

	return FString{DescriptionBuilder};
}

#if WITH_EDITOR
FName UFuBTTask_ActivateAbility::GetNodeIconName() const
{
	return FName{TEXTVIEW("BTEditor.Graph.BTNode.Task.RunEQSQuery.Icon")};
}
#endif

EBTNodeResult::Type UFuBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	if (!FU_ENSURE(AbilityTag.IsValid()))
	{
		return EBTNodeResult::Failed;
	}

	auto& Memory{*CastInstanceNodeMemory<FFuActivateAbilityMemory>(NodeMemory)};

	check(!Memory.AbilitySystem.IsValid())
	check(Memory.ActiveAbilityHandles.IsEmpty())
	check(!Memory.bAnyAbilitySuccessfullyEnded)

	const auto* Controller{BehaviorTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	Memory.AbilitySystem = UFuAbilitySystemUtility::GetAbilitySystem(Pawn);

	if (!FU_ENSURE(Memory.AbilitySystem.IsValid()))
	{
		return EBTNodeResult::Failed;
	}

	const auto AbilityEndedHandle{
		Memory.AbilitySystem->OnAbilityEnded.AddLambda([&Memory](const FAbilityEndedData& EndedData)
		{
			if (Memory.ActiveAbilityHandles.Remove(EndedData.AbilitySpecHandle) > 0)
			{
				Memory.bAnyAbilitySuccessfullyEnded |= !EndedData.bWasCancelled;
			}
		})
	};

	{
		FScopedAbilityListLock AbilityScopeLock{*Memory.AbilitySystem};

		for (const auto& AbilitySpecification : Memory.AbilitySystem->GetActivatableAbilities())
		{
			if (!AbilitySpecification.DynamicAbilityTags.HasTag(AbilityTag) &&
			    !AbilitySpecification.Ability->AbilityTags.HasTag(AbilityTag))
			{
				continue;
			}

			Memory.ActiveAbilityHandles.Emplace(AbilitySpecification.Handle);

			if (!Memory.AbilitySystem->TryActivateAbility(AbilitySpecification.Handle))
			{
				Memory.ActiveAbilityHandles.Remove(AbilitySpecification.Handle);
			}
		}
	}

	Memory.AbilitySystem->OnAbilityEnded.Remove(AbilityEndedHandle);

	if (!bWaitForAbilityEnd)
	{
		return Memory.bAnyAbilitySuccessfullyEnded || !Memory.ActiveAbilityHandles.IsEmpty()
			       ? EBTNodeResult::Succeeded
			       : EBTNodeResult::Failed;
	}

	if (Memory.ActiveAbilityHandles.IsEmpty())
	{
		return Memory.bAnyAbilitySuccessfullyEnded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	Memory.AbilitySystem->OnAbilityEnded.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityEnded,
	                                                TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UFuBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	const auto& Memory{*CastInstanceNodeMemory<FFuActivateAbilityMemory>(NodeMemory)};

	if (bCancelAbilityOnAbort && Memory.AbilitySystem.IsValid())
	{
		Memory.AbilitySystem->OnAbilityEnded.RemoveAll(this);

		for (const auto AbilityHandle : Memory.ActiveAbilityHandles)
		{
			Memory.AbilitySystem->CancelAbilityHandle(AbilityHandle);
		}
	}

	return EBTNodeResult::Aborted;
}

void UFuBTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, const EBTNodeResult::Type Result)
{
	auto& Memory{*CastInstanceNodeMemory<FFuActivateAbilityMemory>(NodeMemory)};

	if (Memory.AbilitySystem.IsValid())
	{
		Memory.AbilitySystem->OnAbilityEnded.RemoveAll(this);
		Memory.AbilitySystem.Reset();
	}

	Memory.ActiveAbilityHandles.Reset();
	Memory.bAnyAbilitySuccessfullyEnded = false;

	Super::OnTaskFinished(BehaviorTree, NodeMemory, Result);
}

void UFuBTTask_ActivateAbility::AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData,
                                                             const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree)
{
	if (FU_ENSURE(BehaviorTree.IsValid()))
	{
		auto& Memory{
			*CastInstanceNodeMemory<FFuActivateAbilityMemory>(
				BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this)))
		};

		if (Memory.ActiveAbilityHandles.Remove(EndedData.AbilitySpecHandle) <= 0)
		{
			return;
		}

		Memory.bAnyAbilitySuccessfullyEnded |= !EndedData.bWasCancelled;

		if (Memory.ActiveAbilityHandles.IsEmpty())
		{
			FinishLatentTask(*BehaviorTree, Memory.bAnyAbilitySuccessfullyEnded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
		}
	}
}
