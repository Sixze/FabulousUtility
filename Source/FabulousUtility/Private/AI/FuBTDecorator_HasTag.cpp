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
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *Tag.ToString());
}

void UFuBTDecorator_HasTag::OnBecomeRelevant(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviourTree, NodeMemory);

	check(!AbilitySystem.IsValid())

	const auto* Controller{BehaviourTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};

	AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);

	if (FU_ENSURE(Tag.IsValid()) && FU_ENSURE(AbilitySystem.IsValid()))
	{
		AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnTagChanged);
	}
}

void UFuBTDecorator_HasTag::OnCeaseRelevant(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory)
{
	if (AbilitySystem.IsValid())
	{
		AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		AbilitySystem = nullptr;
	}

	Super::OnCeaseRelevant(BehaviourTree, NodeMemory);
}

bool UFuBTDecorator_HasTag::CalculateRawConditionValue(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) const
{
	const auto* Controller{BehaviourTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	const auto* ThisAbilitySystem{UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn)};

	return IsValid(ThisAbilitySystem) && ThisAbilitySystem->GetTagCount(Tag) > 0;
}

void UFuBTDecorator_HasTag::OnTagChanged(FGameplayTag ThisTag, int32 NewCount) const
{
	auto* BehaviorTree{Cast<UBehaviorTreeComponent>(GetOuter())};
	if (IsValid(BehaviorTree))
	{
		BehaviorTree->RequestExecution(this);
	}
}
