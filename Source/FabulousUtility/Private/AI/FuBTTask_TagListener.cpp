#include "AI/FuBTTask_TagListener.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "FuMacros.h"

struct FFuTagListenerMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;
};

UFuBTTask_TagListener::UFuBTTask_TagListener()
{
	NodeName = "Fu Wait for Tag Change";
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

uint16 UFuBTTask_TagListener::GetInstanceMemorySize() const
{
	return sizeof(FFuTagListenerMemory);
}

FString UFuBTTask_TagListener::GetStaticDescription() const
{
	switch (WaitMode)
	{
		case EFuTagListenerWaitMode::WaitForTagAdd:
			return FString::Printf(TEXT("Wait for Tag Add: %s"), *Tag.ToString());

		case EFuTagListenerWaitMode::WaitForTagRemove:
			return FString::Printf(TEXT("Wait for Tag Remove: %s"), *Tag.ToString());

		default:
			FU_ENSURE(false);
			return Super::GetStaticDescription();
	}
}

#if WITH_EDITOR
FName UFuBTTask_TagListener::GetNodeIconName() const
{
	return TEXT("BTEditor.Graph.BTNode.Task.Wait.Icon");
}
#endif

EBTNodeResult::Type UFuBTTask_TagListener::ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	if (!FU_ENSURE(Tag.IsValid()) || !FU_ENSURE(FuTagListenerWaitMode::IsValid(WaitMode)))
	{
		return EBTNodeResult::Failed;
	}

	auto& Memory{*CastInstanceNodeMemory<FFuTagListenerMemory>(NodeMemory)};

	check(!Memory.AbilitySystem.IsValid())

	const auto* Controller{BehaviorTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	Memory.AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);

	if (!FU_ENSURE(Memory.AbilitySystem.IsValid()))
	{
		return EBTNodeResult::Failed;
	}

	const auto TagCount{Memory.AbilitySystem->GetTagCount(Tag)};

	// ReSharper disable CppRedundantParentheses
	if ((WaitMode == EFuTagListenerWaitMode::WaitForTagAdd && TagCount > 0) ||
	    (WaitMode == EFuTagListenerWaitMode::WaitForTagRemove && TagCount <= 0))
	// ReSharper restore CppRedundantParentheses
	{
		return EBTNodeResult::Succeeded;
	}

	Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
	      .AddUObject(this, &ThisClass::OnTagChanged, TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});

	return EBTNodeResult::InProgress;
}

void UFuBTTask_TagListener::OnTaskFinished(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, const EBTNodeResult::Type Result)
{
	auto& Memory{*CastInstanceNodeMemory<FFuTagListenerMemory>(NodeMemory)};

	if (Memory.AbilitySystem.IsValid())
	{
		Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		Memory.AbilitySystem = nullptr;
	}

	Super::OnTaskFinished(BehaviorTree, NodeMemory, Result);
}

void UFuBTTask_TagListener::OnTagChanged(const FGameplayTag ThisTag, const int32 NewCount,
                                         const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree) const
{
	// ReSharper disable CppRedundantParentheses
	if ((WaitMode == EFuTagListenerWaitMode::WaitForTagAdd && NewCount > 0 ||
	     WaitMode == EFuTagListenerWaitMode::WaitForTagRemove && NewCount <= 0) &&
	    // ReSharper restore CppRedundantParentheses
	    FU_ENSURE(BehaviorTree.IsValid()))
	{
		FinishLatentTask(*BehaviorTree, EBTNodeResult::Succeeded);
	}
}
