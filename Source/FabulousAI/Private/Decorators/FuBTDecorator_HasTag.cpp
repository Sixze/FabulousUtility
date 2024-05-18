#include "Decorators/FuBTDecorator_HasTag.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTDecorator_HasTag)

struct FFuHasTagMemory
{
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;
};

UFuBTDecorator_HasTag::UFuBTDecorator_HasTag()
{
	NodeName = TEXTVIEW("Fu Has Tag");

	TargetKey.AllowNoneAsValue(true);
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
	TStringBuilder<512> DescriptionBuilder;

	const auto bAborts{FlowAbortMode != EBTFlowAbortMode::None};
	const auto bInversed{bShowInverseConditionDesc && IsInversed()};

	if (bAborts)
	{
		DescriptionBuilder << TEXTVIEW("( aborts ") << *UBehaviorTreeTypes::DescribeFlowAbortMode(FlowAbortMode).ToLower()
			<< (bInversed ? TEXTVIEW(", inversed )") LINE_TERMINATOR : TEXTVIEW(" )") LINE_TERMINATOR);
	}
	else if (bInversed)
	{
		DescriptionBuilder << TEXTVIEW("( inversed )") LINE_TERMINATOR;
	}

	if (Tags.IsEmpty())
	{
		DescriptionBuilder << TEXTVIEW("Has Tag:");
	}
	else if (Tags.Num() == 1)
	{
		DescriptionBuilder << TEXTVIEW("Has Tag: ") << Tags.First().GetTagName();
	}
	else
	{
		DescriptionBuilder << (MatchMode == EFuTagMatchMode::AnyTag ? TEXTVIEW("Has Any Tag:") : TEXTVIEW("Has All Tags:"));

		for (const auto& Tag : Tags)
		{
			DescriptionBuilder << LINE_TERMINATOR << Tag.GetTagName();
		}
	}

	DescriptionBuilder << LINE_TERMINATOR TEXTVIEW("Target: ") << TargetKey.SelectedKeyName;

	return FString{DescriptionBuilder};
}

void UFuBTDecorator_HasTag::OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviorTree, NodeMemory);

	auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (TargetKey.IsSet() && FU_ENSURE(IsValid(Blackboard)))
	{
		Blackboard->RegisterObserver(TargetKey.GetSelectedKeyID(), this,
		                             FOnBlackboardChangeNotification::CreateUObject(this, &ThisClass::Blackboard_OnTargetKeyChanged));
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
			                ? UFuAbilitySystemUtility::GetAbilitySystem(
				                Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID()))
			                : nullptr;

		if (!IsValid(AbilitySystem))
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

	const auto* TargetActor{Cast<AActor>(Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID()))};
	if (!IsValid(TargetActor))
	{
		return;
	}

	Memory.AbilitySystem = UFuAbilitySystemUtility::GetAbilitySystem(TargetActor);
	if (!FU_ENSURE(Memory.AbilitySystem.IsValid()))
	{
		return;
	}

	for (const auto& Tag : Tags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Memory.AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
			      .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged, TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});
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

	Memory.AbilitySystem.Reset();
}

EBlackboardNotificationResult UFuBTDecorator_HasTag::Blackboard_OnTargetKeyChanged(const UBlackboardComponent& Blackboard,
                                                                                   FBlackboard::FKey Key)
{
	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	if (!FU_ENSURE(IsValid(BehaviorTree)))
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	auto* NodeMemory{BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this))};
	auto& Memory{*CastInstanceNodeMemory<FFuHasTagMemory>(NodeMemory)};

	ReInitializeDecoratorMemory(*BehaviorTree, Memory);

	if (CalculateRawConditionValue(*BehaviorTree, NodeMemory) == IsInversed())
	{
		BehaviorTree->RequestBranchDeactivation(*this);
	}
	else
	{
		BehaviorTree->RequestBranchActivation(*this, false);
	}

	return EBlackboardNotificationResult::ContinueObserving;
}

void UFuBTDecorator_HasTag::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 TagCount,
                                                       const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree)
{
	if (FU_ENSURE(BehaviorTree.IsValid()))
	{
		auto* NodeMemory{BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this))};

		if (CalculateRawConditionValue(*BehaviorTree, NodeMemory) == IsInversed())
		{
			BehaviorTree->RequestBranchDeactivation(*this);
		}
		else
		{
			BehaviorTree->RequestBranchActivation(*this, false);
		}
	}
}
