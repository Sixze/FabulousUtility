#include "AI/FuBTTask_TagListener.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "FuMacros.h"

UFuBTTask_TagListener::UFuBTTask_TagListener()
{
	NodeName = "Wait for Tag Change";
	bCreateNodeInstance = true;
	bIgnoreRestartSelf = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

#if WITH_EDITOR
bool UFuBTTask_TagListener::CanEditChange(const FProperty* Property) const
{
	return Super::CanEditChange(Property) &&
	       Property->GetFName() != GET_MEMBER_NAME_CHECKED(ThisClass, bIgnoreRestartSelf);
}
#endif

FString UFuBTTask_TagListener::GetStaticDescription() const
{
	switch (WaitMode)
	{
		case EFuTagListenerWaitMode::WaitForTagAdd:
			return FString::Printf(TEXT("Wait for Tag Add: %s"), *Tag.ToString());

		case EFuTagListenerWaitMode::WaitForTagRemove:
			return FString::Printf(TEXT("Wait for Tag Remove: %s"), *Tag.ToString());

		default:
			return Super::GetStaticDescription();
	}
}

#if WITH_EDITOR
FName UFuBTTask_TagListener::GetNodeIconName() const
{
	return TEXT("BTEditor.Graph.BTNode.Task.Wait.Icon");
}
#endif

EBTNodeResult::Type UFuBTTask_TagListener::ExecuteTask(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	check(!AbilitySystem.IsValid())

	const auto* Controller{BehaviourTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};

	AbilitySystem = Cast<UAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));

	if (!FU_ENSURE(Tag.IsValid()) || !FU_ENSURE(FuTagListenerWaitMode::IsValid(WaitMode)) || !FU_ENSURE(AbilitySystem.IsValid()))
	{
		return EBTNodeResult::Failed;
	}

	const auto TagCount{AbilitySystem->GetTagCount(Tag)};

	if (WaitMode == EFuTagListenerWaitMode::WaitForTagAdd && TagCount > 0 ||
	    WaitMode == EFuTagListenerWaitMode::WaitForTagRemove && TagCount <= 0)
	{
		return EBTNodeResult::Succeeded;
	}

	AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnTagChanged);

	return EBTNodeResult::InProgress;
}

void UFuBTTask_TagListener::OnTaskFinished(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory, const EBTNodeResult::Type Result)
{
	if (AbilitySystem.IsValid())
	{
		AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		AbilitySystem = nullptr;
	}

	Super::OnTaskFinished(BehaviourTree, NodeMemory, Result);
}

void UFuBTTask_TagListener::OnTagChanged(const FGameplayTag ThisTag, const int32 NewCount) const
{
	if (WaitMode == EFuTagListenerWaitMode::WaitForTagAdd && NewCount > 0 ||
	    WaitMode == EFuTagListenerWaitMode::WaitForTagRemove && NewCount <= 0)
	{
		auto* BehaviorTree{Cast<UBehaviorTreeComponent>(GetOuter())};
		if (IsValid(BehaviorTree))
		{
			FinishLatentTask(*BehaviorTree, EBTNodeResult::Succeeded);
		}
	}
}
