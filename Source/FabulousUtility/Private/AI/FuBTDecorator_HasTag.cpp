#include "AI/FuBTDecorator_HasTag.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "FuMacros.h"

UFuBTDecorator_HasTag::UFuBTDecorator_HasTag()
{
	NodeName = "Has Tag";
	bCreateNodeInstance = true;

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

FString UFuBTDecorator_HasTag::GetStaticDescription() const
{
	FString Description;

	const auto bAborts{FlowAbortMode != EBTFlowAbortMode::None};
	const auto bInversed{bShowInverseConditionDesc && IsInversed()};

	if (bAborts)
	{
		Description = FString::Printf(TEXT("( aborts %s%s )"),
		                              *UBehaviorTreeTypes::DescribeFlowAbortMode(FlowAbortMode).ToLower(),
		                              bInversed ? TEXT(", inversed") : TEXT(""));
	}
	else if (bInversed)
	{
		Description = TEXT("( inversed )");
	}

	if (Tags.Num() <= 0)
	{
		Description += LINE_TERMINATOR TEXT("Has Tag: ");
	}
	else if (Tags.Num() == 1)
	{
		Description += LINE_TERMINATOR TEXT("Has Tag: ") + Tags.First().ToString();
	}
	else
	{
		Description += TagsMatchMode == EGameplayContainerMatchType::All ? TEXT(" Has All Tags: ") : TEXT(" Has Any Tag: ");

		for (const auto& Tag : Tags)
		{
			Description += LINE_TERMINATOR + Tag.ToString();
		}
	}

	return Description;
}

void UFuBTDecorator_HasTag::OnBecomeRelevant(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviourTree, NodeMemory);

	check(!AbilitySystem.IsValid())

	const auto* Controller{BehaviourTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};

	AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);

	if (!FU_ENSURE(AbilitySystem.IsValid()))
	{
		return;
	}

	for (const auto& Tag : Tags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnTagChanged);
		}
	}
}

void UFuBTDecorator_HasTag::OnCeaseRelevant(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	if (AbilitySystem.IsValid())
	{
		for (const auto& Tag : Tags)
		{
			if (Tag.IsValid())
			{
				AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
			}
		}

		AbilitySystem = nullptr;
	}

	Super::OnCeaseRelevant(BehaviourTree, NodeMemory);
}

bool UFuBTDecorator_HasTag::CalculateRawConditionValue(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) const
{
	const auto* Controller{BehaviourTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	const auto* ThisAbilitySystem{UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn)};

	if (!IsValid(ThisAbilitySystem))
	{
		return false;
	}

	switch (TagsMatchMode)
	{
		case EGameplayContainerMatchType::All:
			return ThisAbilitySystem->HasAllMatchingGameplayTags(Tags);

		case EGameplayContainerMatchType::Any:
			return ThisAbilitySystem->HasAnyMatchingGameplayTags(Tags);

		default:
			return false;
	}
}

void UFuBTDecorator_HasTag::OnTagChanged(FGameplayTag ThisTag, int32 NewCount) const
{
	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(GetOuter())};
	if (IsValid(BehaviorTree))
	{
		BehaviorTree->RequestExecution(this);
	}
}
