#include "AI/FuBTDecorator_CanActivateAbility.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTDecorator_CanActivateAbility)

struct FFuCanActivateAbilityMemory
{
	TWeakObjectPtr<UFuAbilitySystemComponent> AbilitySystem;

	TSet<FGameplayAbilitySpecHandle> AbilityHandles;
};

UFuBTDecorator_CanActivateAbility::UFuBTDecorator_CanActivateAbility()
{
	NodeName = TEXTVIEW("Fu Can Activate Ability");

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

FString UFuBTDecorator_CanActivateAbility::GetStaticDescription() const
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

	if (AbilityTags.IsEmpty())
	{
		DescriptionBuilder << TEXTVIEW("Can Activate Ability:");
	}
	else if (AbilityTags.Num() == 1)
	{
		DescriptionBuilder << TEXT("Can Activate Ability: ") << AbilityTags.First().GetTagName();
	}
	else
	{
		DescriptionBuilder << TEXTVIEW("Can Activate Any Ability: ");

		for (const auto& Tag : AbilityTags)
		{
			DescriptionBuilder << LINE_TERMINATOR << Tag.GetTagName();
		}
	}

	return FString{DescriptionBuilder};
}

uint16 UFuBTDecorator_CanActivateAbility::GetInstanceMemorySize() const
{
	return sizeof(FFuCanActivateAbilityMemory);
}

void UFuBTDecorator_CanActivateAbility::OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(BehaviorTree, NodeMemory);

	auto& Memory{*CastInstanceNodeMemory<FFuCanActivateAbilityMemory>(NodeMemory)};

	const auto* Controller{BehaviorTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	Memory.AbilitySystem = Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));

	if (!FU_ENSURE(Memory.AbilitySystem.IsValid()))
	{
		return;
	}

	Memory.AbilitySystem->OnAbilityGiven.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityGiven,
	                                                TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});

	Memory.AbilitySystem->OnAbilityRemoved.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityRemoved,
	                                                  TWeakObjectPtr<UBehaviorTreeComponent>{&BehaviorTree});

	for (const auto& AbilitySpecification : Memory.AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
		    AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags))
		{
			Memory.AbilityHandles.Add(AbilitySpecification.Handle);
		}
	}
}

void UFuBTDecorator_CanActivateAbility::OnCeaseRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	auto& Memory{*CastInstanceNodeMemory<FFuCanActivateAbilityMemory>(NodeMemory)};

	if (Memory.AbilitySystem.IsValid())
	{
		Memory.AbilitySystem->OnAbilityGiven.RemoveAll(this);
		Memory.AbilitySystem->OnAbilityRemoved.RemoveAll(this);
		Memory.AbilitySystem = nullptr;
	}

	Memory.AbilityHandles.Reset();

	Super::OnCeaseRelevant(BehaviorTree, NodeMemory);
}

void UFuBTDecorator_CanActivateAbility::TickNode(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, const float DeltaTime)
{
	Super::TickNode(BehaviorTree, NodeMemory, DeltaTime);

	const auto bExecutingBranch{BehaviorTree.IsExecutingBranch(GetMyNode(), GetChildIndex())};

	// ReSharper disable CppRedundantParentheses
	const auto bAbortAllowed{
		(bExecutingBranch && ((FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both) &&
		                      CalculateRawConditionValue(BehaviorTree, NodeMemory) == IsInversed())) ||
		(!bExecutingBranch && ((FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both) &&
		                       CalculateRawConditionValue(BehaviorTree, NodeMemory) != IsInversed()))

	};
	// ReSharper restore CppRedundantParentheses

	if (bAbortAllowed)
	{
		BehaviorTree.RequestExecution(this);
	}
}

bool UFuBTDecorator_CanActivateAbility::CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) const
{
	const auto& Memory{*CastInstanceNodeMemory<FFuCanActivateAbilityMemory>(NodeMemory)};

	if (Memory.AbilitySystem.IsValid())
	{
		for (const auto& AbilitySpecification : Memory.AbilitySystem->GetActivatableAbilities())
		{
			if (Memory.AbilityHandles.Contains(AbilitySpecification.Handle) &&
			    AbilitySpecification.Ability->CanActivateAbility(AbilitySpecification.Handle, Memory.AbilitySystem->AbilityActorInfo.Get()))
			{
				return true;
			}
		}

		return false;
	}

	const auto* Controller{BehaviorTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};
	const auto* AbilitySystem{UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn)};

	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if ((AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
		     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)) &&
		    AbilitySpecification.Ability->CanActivateAbility(AbilitySpecification.Handle, AbilitySystem->AbilityActorInfo.Get()))
		{
			return true;
		}
	}

	return false;
}

void UFuBTDecorator_CanActivateAbility::AbilitySystem_OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpecification,
                                                                     const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree)
{
	if (FU_ENSURE(BehaviorTree.IsValid()) &&
	    (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)))
	{
		auto& Memory{
			*CastInstanceNodeMemory<FFuCanActivateAbilityMemory>(
				BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this)))
		};

		Memory.AbilityHandles.Add(AbilitySpecification.Handle);
	}
}

void UFuBTDecorator_CanActivateAbility::AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification,
                                                                       const TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree)
{
	if (FU_ENSURE(BehaviorTree.IsValid()) &&
	    (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)))
	{
		auto& Memory{
			*CastInstanceNodeMemory<FFuCanActivateAbilityMemory>(
				BehaviorTree->GetNodeMemory(this, BehaviorTree->FindInstanceContainingNode(this)))
		};

		Memory.AbilityHandles.Remove(AbilitySpecification.Handle);
	}
}
