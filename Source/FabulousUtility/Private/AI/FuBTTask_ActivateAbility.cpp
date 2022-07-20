#include "AI/FuBTTask_ActivateAbility.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "FuMacros.h"

UFuBTTask_ActivateAbility::UFuBTTask_ActivateAbility()
{
	NodeName = "Activate Ability";
	bCreateNodeInstance = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

FString UFuBTTask_ActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *AbilityTag.ToString());
}

#if WITH_EDITOR
FName UFuBTTask_ActivateAbility::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.RunEQSQuery.Icon");
}
#endif

EBTNodeResult::Type UFuBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	check(!AbilitySystem.IsValid())
	check(ActiveAbilityHandles.IsEmpty())
	check(!bAnyAbilitySuccessfullyEnded)

	const auto* Controller{BehaviourTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};

	AbilitySystem = Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));

	if (!FU_ENSURE(AbilityTag.IsValid()) || !FU_ENSURE(AbilitySystem.IsValid()))
	{
		return EBTNodeResult::Failed;
	}

	AbilitySystem->OnAbilityActivated.AddDynamic(this, &ThisClass::OnAbilityActivated);

	const auto AbilityEndedHandle{
		AbilitySystem->OnAbilityEnded.AddWeakLambda(this, [this](const FAbilityEndedData& EndedData)
		{
			if (ActiveAbilityHandles.Remove(EndedData.AbilitySpecHandle) > 0)
			{
				bAnyAbilitySuccessfullyEnded |= !EndedData.bWasCancelled;
			}
		})
	};

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.Ability->AbilityTags.HasTag(AbilityTag))
		{
			AbilitySystem->TryActivateAbility(AbilitySpecification.Handle);
		}
	}

	AbilitySystem->OnAbilityEnded.Remove(AbilityEndedHandle);
	AbilitySystem->OnAbilityActivated.RemoveAll(this);

	if (!bWaitForAbilityEnd)
	{
		return bAnyAbilitySuccessfullyEnded || ActiveAbilityHandles.Num() > 0 ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	if (ActiveAbilityHandles.IsEmpty())
	{
		return bAnyAbilitySuccessfullyEnded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	AbilitySystem->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnded);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UFuBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	if (bCancelAbilityOnAbort && AbilitySystem.IsValid())
	{
		for (const auto AbilityHandle : ActiveAbilityHandles)
		{
			AbilitySystem->CancelAbilityHandle(AbilityHandle);
		}
	}

	return EBTNodeResult::Aborted;
}

void UFuBTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory,
                                               const EBTNodeResult::Type Result)
{
	Super::OnTaskFinished(BehaviourTree, NodeMemory, Result);

	if (AbilitySystem.IsValid())
	{
		AbilitySystem->OnAbilityEnded.RemoveAll(this);
		AbilitySystem = nullptr;
	}

	ActiveAbilityHandles.Reset();
	bAnyAbilitySuccessfullyEnded = false;
}

void UFuBTTask_ActivateAbility::OnAbilityActivated(const FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability)
{
	ActiveAbilityHandles.Add(AbilityHandle);
}

void UFuBTTask_ActivateAbility::OnAbilityEnded(const FAbilityEndedData& EndedData)
{
	if (ActiveAbilityHandles.Remove(EndedData.AbilitySpecHandle) <= 0)
	{
		return;
	}

	bAnyAbilitySuccessfullyEnded |= !EndedData.bWasCancelled;

	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(GetOuter())};

	if (ActiveAbilityHandles.IsEmpty() && IsValid(BehaviorTree))
	{
		FinishLatentTask(*BehaviorTree, bAnyAbilitySuccessfullyEnded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
}
