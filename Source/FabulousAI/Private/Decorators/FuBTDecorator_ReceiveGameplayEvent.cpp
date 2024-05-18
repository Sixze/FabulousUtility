#include "Decorators/FuBTDecorator_ReceiveGameplayEvent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTDecorator_ReceiveGameplayEvent)

struct FFuReceiveGameplayEventMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;

	int32 EventReceivedCounter{0};
};

UFuBTDecorator_ReceiveGameplayEvent::UFuBTDecorator_ReceiveGameplayEvent()
{
	NodeName = TEXTVIEW("Fu Receive Gameplay Event");

	TargetKey.AllowNoneAsValue(true);
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetKey), AActor::StaticClass());

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UFuBTDecorator_ReceiveGameplayEvent::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const auto* BlackboardData{GetBlackboardAsset()};
	if (FU_ENSURE(BlackboardData))
	{
		TargetKey.ResolveSelectedKey(*BlackboardData);
	}
}

uint16 UFuBTDecorator_ReceiveGameplayEvent::GetInstanceMemorySize() const
{
	return sizeof(FFuReceiveGameplayEventMemory);
}

FString UFuBTDecorator_ReceiveGameplayEvent::GetStaticDescription() const
{
	TStringBuilder<512> DescriptionBuilder;

	if (FlowAbortMode != EBTFlowAbortMode::None)
	{
		DescriptionBuilder << TEXTVIEW("( aborts ") << *UBehaviorTreeTypes::DescribeFlowAbortMode(FlowAbortMode).ToLower()
			<< TEXTVIEW(" )") LINE_TERMINATOR;
	}

	if (EventTags.IsEmpty())
	{
		DescriptionBuilder << TEXTVIEW("Receive Gameplay Event: ");
	}
	else if (EventTags.Num() == 1)
	{
		DescriptionBuilder << TEXTVIEW("Receive Gameplay Event: ") << EventTags.First().GetTagName();
	}
	else
	{
		DescriptionBuilder << TEXTVIEW("Receive Any Gameplay Event: ");

		for (const auto& Tag : EventTags)
		{
			DescriptionBuilder << LINE_TERMINATOR << Tag.GetTagName();
		}
	}

	DescriptionBuilder << LINE_TERMINATOR TEXTVIEW("Target: ") << TargetKey.SelectedKeyName;

	return FString{DescriptionBuilder};
}

void UFuBTDecorator_ReceiveGameplayEvent::OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviorTree, NodeMemory);

	auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (TargetKey.IsSet() && FU_ENSURE(IsValid(Blackboard)))
	{
		Blackboard->RegisterObserver(TargetKey.GetSelectedKeyID(), this,
		                             FOnBlackboardChangeNotification::CreateUObject(this, &ThisClass::Blackboard_OnTargetKeyChanged));
	}

	ReInitializeDecoratorMemory(BehaviorTree, *CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(NodeMemory));
}

void UFuBTDecorator_ReceiveGameplayEvent::OnCeaseRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (FU_ENSURE(IsValid(Blackboard)))
	{
		Blackboard->UnregisterObserversFrom(this);
	}

	ClearDecoratorMemory(*CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(NodeMemory));

	Super::OnCeaseRelevant(BehaviorTree, NodeMemory);
}

bool UFuBTDecorator_ReceiveGameplayEvent::CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) const
{
	if (FlowAbortMode == EBTFlowAbortMode::Self || bAllowEntryInNonSelfFlowAbortMode)
	{
		return true;
	}

	auto& Memory{*CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(NodeMemory)};
	if (Memory.EventReceivedCounter > 0)
	{
		Memory.EventReceivedCounter -= 1;
		return true;
	}

	return false;
}

void UFuBTDecorator_ReceiveGameplayEvent::ReInitializeDecoratorMemory(UBehaviorTreeComponent& BehaviorTree,
                                                                      FFuReceiveGameplayEventMemory& Memory)
{
	ClearDecoratorMemory(Memory);

	const auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (!FU_ENSURE(IsValid(Blackboard)))
	{
		return;
	}

	const auto* TargetActor{Cast<AActor>(Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID()))};
	if (!IsValid(TargetActor))
	{
		return;
	}

	Memory.AbilitySystem = UFuAbilitySystemUtility::GetAbilitySystem(TargetActor);
	if (!Memory.AbilitySystem.IsValid())
	{
		return;
	}

	for (const auto& Tag : EventTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Memory.AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Tag)
			      .AddUObject(this, &ThisClass::AbilitySystem_OnEventReceived, TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});
		}
	}
}

void UFuBTDecorator_ReceiveGameplayEvent::ClearDecoratorMemory(FFuReceiveGameplayEventMemory& Memory)
{
	if (!Memory.AbilitySystem.IsValid())
	{
		return;
	}

	for (const auto& Tag : EventTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Memory.AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Tag).RemoveAll(this);
		}
	}

	Memory.AbilitySystem.Reset();
	Memory.EventReceivedCounter = 0;
}

EBlackboardNotificationResult UFuBTDecorator_ReceiveGameplayEvent::Blackboard_OnTargetKeyChanged(
	const UBlackboardComponent& Blackboard, const FBlackboard::FKey Key)
{
	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	if (!FU_ENSURE(IsValid(BehaviorTree)))
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	ReInitializeDecoratorMemory(*BehaviorTree, *CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(
		                            BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this))));

	return EBlackboardNotificationResult::ContinueObserving;
}

void UFuBTDecorator_ReceiveGameplayEvent::AbilitySystem_OnEventReceived(const FGameplayEventData* EventData,
                                                                        const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree)
{
	if (!FU_ENSURE(BehaviorTree.IsValid()))
	{
		return;
	}

	if (BehaviorTree->IsExecutingBranch(GetMyNode(), GetChildIndex()))
	{
		if (FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both)
		{
			BehaviorTree->RequestBranchDeactivation(*this);
		}

		return;
	}

	if (FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both)
	{
		if (!bAllowEntryInNonSelfFlowAbortMode)
		{
			auto& Memory{
				*CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(
					BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this)))
			};

			Memory.EventReceivedCounter = 2;
		}

		BehaviorTree->RequestBranchActivation(*this, false);
	}
}
