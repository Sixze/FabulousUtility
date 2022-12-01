#include "AI/FuBTDecorator_HasTag.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

struct FFuHasTagMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;
};

UFuBTDecorator_HasTag::UFuBTDecorator_HasTag()
{
	NodeName = TEXT("Fu Has Tag");

	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetKey), AActor::StaticClass());

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UFuBTDecorator_HasTag::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const auto* BlackboardData{GetBlackboardAsset()};
	if (FU_ENSURE(BlackboardData))
	{
		TargetKey.ResolveSelectedKey(*BlackboardData);
	}
}

uint16 UFuBTDecorator_HasTag::GetInstanceMemorySize() const
{
	return sizeof(FFuHasTagMemory);
}

FString UFuBTDecorator_HasTag::GetStaticDescription() const
{
	FString Description;

	const auto bAborts{FlowAbortMode != EBTFlowAbortMode::None};
	const auto bInversed{bShowInverseConditionDesc && IsInversed()};

	if (bAborts)
	{
		Description = FString::Printf(TEXT("( aborts %s%s )") LINE_TERMINATOR,
		                              *UBehaviorTreeTypes::DescribeFlowAbortMode(FlowAbortMode).ToLower(),
		                              bInversed ? TEXT(", inversed") : TEXT(""));
	}
	else if (bInversed)
	{
		Description = TEXT("( inversed )");
	}

	if (Tags.Num() <= 0)
	{
		Description += TEXT("Has Tag: ");
	}
	else if (Tags.Num() == 1)
	{
		Description += TEXT("Has Tag: ") + Tags.First().ToString();
	}
	else
	{
		Description += MatchMode == EFuTagMatchMode::AnyTag ? TEXT("Has Any Tag: ") : TEXT("Has All Tags: ");

		for (const auto& Tag : Tags)
		{
			Description += LINE_TERMINATOR + Tag.ToString();
		}
	}

	Description += LINE_TERMINATOR TEXT("Target: ") + TargetKey.SelectedKeyName.ToString();

	return Description;
}

void UFuBTDecorator_HasTag::OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviorTree, NodeMemory);

	auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (FU_ENSURE(IsValid(Blackboard)))
	{
		Blackboard->RegisterObserver(TargetKey.GetSelectedKeyID(), this,
		                             FOnBlackboardChangeNotification::CreateUObject(this, &ThisClass::OnTargetKeyChanged));
	}

	ReInitializeDecoratorMemory(BehaviorTree, *CastInstanceNodeMemory<FFuHasTagMemory>(NodeMemory));
}

void UFuBTDecorator_HasTag::OnCeaseRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (FU_ENSURE(IsValid(Blackboard)))
	{
		Blackboard->UnregisterObserversFrom(this);
	}

	ClearDecoratorMemory(*CastInstanceNodeMemory<FFuHasTagMemory>(NodeMemory));

	Super::OnCeaseRelevant(BehaviorTree, NodeMemory);
}

bool UFuBTDecorator_HasTag::CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) const
{
	const auto& Memory{*CastInstanceNodeMemory<FFuHasTagMemory>(NodeMemory)};
	const UAbilitySystemComponent* AbilitySystem;

	if (Memory.AbilitySystem.IsValid())
	{
		AbilitySystem = Memory.AbilitySystem.Get();
	}
	else
	{
		const auto* Blackboard{BehaviorTree.GetBlackboardComponent()};

		AbilitySystem = FU_ENSURE(IsValid(Blackboard))
			                ? UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Cast<AActor>(
				                Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID())))
			                : nullptr;

		if (!FU_ENSURE(IsValid(AbilitySystem)))
		{
			return false;
		}
	}

	switch (MatchMode)
	{
		case EFuTagMatchMode::AnyTag:
			return AbilitySystem->HasAnyMatchingGameplayTags(Tags);

		case EFuTagMatchMode::AllTags:
			return AbilitySystem->HasAllMatchingGameplayTags(Tags);

		default:
			FU_ENSURE(false);
			return false;
	}
}

void UFuBTDecorator_HasTag::ReInitializeDecoratorMemory(UBehaviorTreeComponent& BehaviorTree, FFuHasTagMemory& Memory)
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

	for (const auto& Tag : Tags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
			      .AddUObject(this, &ThisClass::OnTagChanged, TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});
		}
	}
}

void UFuBTDecorator_HasTag::ClearDecoratorMemory(FFuHasTagMemory& Memory)
{
	if (!Memory.AbilitySystem.IsValid())
	{
		return;
	}

	for (const auto& Tag : Tags)
	{
		if (Tag.IsValid())
		{
			Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	Memory.AbilitySystem = nullptr;
}

EBlackboardNotificationResult UFuBTDecorator_HasTag::OnTargetKeyChanged(const UBlackboardComponent& Blackboard, FBlackboard::FKey Key)
{
	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	if (!FU_ENSURE(IsValid(BehaviorTree)))
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	ReInitializeDecoratorMemory(*BehaviorTree, *CastInstanceNodeMemory<FFuHasTagMemory>(
		                            BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this))));

	BehaviorTree->RequestExecution(this);

	return EBlackboardNotificationResult::ContinueObserving;
}

void UFuBTDecorator_HasTag::OnTagChanged(FGameplayTag Tag, int32 NewCount,
                                         const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree) const
{
	if (FU_ENSURE(BehaviorTree.IsValid()))
	{
		BehaviorTree->RequestExecution(this);
	}
}
