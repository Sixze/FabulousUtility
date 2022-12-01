#include "AI/FuBTDecorator_ReceiveGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

struct FFuReceiveGameplayEventMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;

	int32 EventReceivedCounter;
};

UFuBTDecorator_ReceiveGameplayEvent::UFuBTDecorator_ReceiveGameplayEvent()
{
	NodeName = TEXT("Fu Receive Gameplay Event");
	bAllowAbortNone = false;
	FlowAbortMode = EBTFlowAbortMode::LowerPriority;

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
	auto Description{
		TEXT("( aborts ") + UBehaviorTreeTypes::DescribeFlowAbortMode(FlowAbortMode).ToLower() + TEXT(" )") LINE_TERMINATOR
	};

	if (EventTags.Num() <= 0)
	{
		Description += TEXT("Receive Gameplay Event: ");
	}
	else if (EventTags.Num() == 1)
	{
		Description += TEXT("Receive Gameplay Event: ") + EventTags.First().ToString();
	}
	else
	{
		Description += TEXT("Receive Any Gameplay Events: ");

		for (const auto& Tag : EventTags)
		{
			Description += LINE_TERMINATOR + Tag.ToString();
		}
	}

	Description += LINE_TERMINATOR TEXT("Target: ") + TargetKey.SelectedKeyName.ToString();

	return Description;
}

void UFuBTDecorator_ReceiveGameplayEvent::OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviorTree, NodeMemory);

	auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (FU_ENSURE(IsValid(Blackboard)))
	{
		Blackboard->RegisterObserver(TargetKey.GetSelectedKeyID(), this,
		                             FOnBlackboardChangeNotification::CreateUObject(this, &ThisClass::OnTargetKeyChanged));
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
	if (FlowAbortMode != EBTFlowAbortMode::LowerPriority)
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

	Memory.AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
		Cast<AActor>(Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID())));

	if (!FU_ENSURE(Memory.AbilitySystem.IsValid()))
	{
		return;
	}

	for (const auto& Tag : EventTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Memory.AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(Tag)
			      .AddUObject(this, &ThisClass::OnEventReceived, TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});
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

	Memory.AbilitySystem = nullptr;
	Memory.EventReceivedCounter = 0;
}

EBlackboardNotificationResult UFuBTDecorator_ReceiveGameplayEvent::OnTargetKeyChanged(const UBlackboardComponent& Blackboard,
                                                                                      FBlackboard::FKey Key)
{
	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	if (!FU_ENSURE(IsValid(BehaviorTree)))
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	ReInitializeDecoratorMemory(*BehaviorTree, *CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(
		                            BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this))));

	BehaviorTree->RequestExecution(this);

	return EBlackboardNotificationResult::ContinueObserving;
}

void UFuBTDecorator_ReceiveGameplayEvent::OnEventReceived(const FGameplayEventData* Payload,
                                                          const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree)
{
	if (!FU_ENSURE(BehaviorTree.IsValid()))
	{
		return;
	}

	if (FlowAbortMode == EBTFlowAbortMode::LowerPriority)
	{
		auto& Memory{
			*CastInstanceNodeMemory<FFuReceiveGameplayEventMemory>(
				BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this)))
		};

		Memory.EventReceivedCounter = 2;
	}

	BehaviorTree->RequestExecution(this);
}
