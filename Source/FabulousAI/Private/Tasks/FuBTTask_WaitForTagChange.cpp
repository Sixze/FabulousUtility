#include "Tasks/FuBTTask_WaitForTagChange.h"

#include "AIController.h"
#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTTask_WaitForTagChange)

struct FFuTagListenerMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;
};

UFuBTTask_WaitForTagChange::UFuBTTask_WaitForTagChange()
{
	NodeName = TEXTVIEW("Fu Wait for Tag Change");
	bIgnoreRestartSelf = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

#if WITH_EDITOR
bool UFuBTTask_WaitForTagChange::CanEditChange(const FProperty* Property) const
{
	return Super::CanEditChange(Property) &&
	       Property->GetFName() != GET_MEMBER_NAME_CHECKED(ThisClass, bIgnoreRestartSelf);
}
#endif

uint16 UFuBTTask_WaitForTagChange::GetInstanceMemorySize() const
{
	return sizeof(FFuTagListenerMemory);
}

FString UFuBTTask_WaitForTagChange::GetStaticDescription() const
{
	TStringBuilder<256> DescriptionBuilder;

	switch (WaitMode)
	{
		case EFuTagWaitMode::WaitForTagAdd:
			DescriptionBuilder << TEXTVIEW("Wait for Tag Add: ");
			break;

		case EFuTagWaitMode::WaitForTagRemove:
			DescriptionBuilder << TEXTVIEW("Wait for Tag Remove: ");
			break;

		default:
			FU_ENSURE(false);
			return Super::GetStaticDescription();
	}

	if (Tag.IsValid())
	{
		DescriptionBuilder << Tag.GetTagName();
	}

	return FString{DescriptionBuilder};
}

#if WITH_EDITOR
FName UFuBTTask_WaitForTagChange::GetNodeIconName() const
{
	return FName{TEXTVIEW("BTEditor.Graph.BTNode.Task.Wait.Icon")};
}
#endif

EBTNodeResult::Type UFuBTTask_WaitForTagChange::ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	if (!FU_ENSURE(Tag.IsValid()) || !FU_ENSURE(FuTagListenerWaitMode::IsValid(WaitMode)))
	{
		return EBTNodeResult::Failed;
	}

	auto& Memory{*CastInstanceNodeMemory<FFuTagListenerMemory>(NodeMemory)};

	check(!Memory.AbilitySystem.IsValid())

	const auto* Controller{BehaviorTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	Memory.AbilitySystem = UFuAbilitySystemUtility::GetAbilitySystem(Pawn);

	if (!FU_ENSURE(Memory.AbilitySystem.IsValid()))
	{
		return EBTNodeResult::Failed;
	}

	const auto TagCount{Memory.AbilitySystem->GetTagCount(Tag)};

	if ((WaitMode == EFuTagWaitMode::WaitForTagAdd && TagCount > 0) ||
	    (WaitMode == EFuTagWaitMode::WaitForTagRemove && TagCount <= 0))
	{
		return EBTNodeResult::Succeeded;
	}

	Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
	      .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged, TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});

	return EBTNodeResult::InProgress;
}

void UFuBTTask_WaitForTagChange::OnTaskFinished(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, const EBTNodeResult::Type Result)
{
	auto& Memory{*CastInstanceNodeMemory<FFuTagListenerMemory>(NodeMemory)};

	if (Memory.AbilitySystem.IsValid())
	{
		Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		Memory.AbilitySystem.Reset();
	}

	Super::OnTaskFinished(BehaviorTree, NodeMemory, Result);
}

void UFuBTTask_WaitForTagChange::AbilitySystem_OnTagChanged(const FGameplayTag OtherTag, const int32 TagCount,
                                                            const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree) const
{
	if (((WaitMode == EFuTagWaitMode::WaitForTagAdd && TagCount > 0) ||
	     (WaitMode == EFuTagWaitMode::WaitForTagRemove && TagCount <= 0)) &&
	    FU_ENSURE(BehaviorTree.IsValid()))
	{
		FinishLatentTask(*BehaviorTree, EBTNodeResult::Succeeded);
	}
}
