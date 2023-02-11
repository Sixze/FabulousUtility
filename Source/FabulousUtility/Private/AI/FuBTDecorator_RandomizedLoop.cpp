#include "AI/FuBTDecorator_RandomizedLoop.h"

#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTDecorator_RandomizedLoop)

struct FFuRandomizedLoopMemory
{
	uint8 RemainingLoopsCount{0};
};

UFuBTDecorator_RandomizedLoop::UFuBTDecorator_RandomizedLoop()
{
	NodeName = TEXT("Fu Randomized Loop");

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

#if WITH_EDITOR
void UFuBTDecorator_RandomizedLoop::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, MinLoopsCount))
	{
		MinLoopsCount = FMath::Min(MinLoopsCount, MaxLoopsCount);
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, MaxLoopsCount))
	{
		MaxLoopsCount = FMath::Max(MinLoopsCount, MaxLoopsCount);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UFuBTDecorator_RandomizedLoop::DescribeRuntimeValues(const UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory,
                                                          const EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(BehaviorTree, NodeMemory, Verbosity, Values);

	const auto& Memory{*CastInstanceNodeMemory<FFuRandomizedLoopMemory>(NodeMemory)};

	Values.Add(FString::Format(TEXT("Loops remaining: {0}"), {Memory.RemainingLoopsCount}));
}

uint16 UFuBTDecorator_RandomizedLoop::GetInstanceMemorySize() const
{
	return sizeof(FFuRandomizedLoopMemory);
}

FString UFuBTDecorator_RandomizedLoop::GetStaticDescription() const
{
	return FString::Format(TEXT("Randomized Loop: {1}-{2} loops"), {*Super::GetStaticDescription(), MinLoopsCount, MaxLoopsCount});
}

void UFuBTDecorator_RandomizedLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	const auto& ParentMemory{*GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData)};
	const auto bParentIsSpecialNode{GetParentNode()->IsA<UBTComposite_SimpleParallel>()};

	auto& Memory{*GetNodeMemory<FFuRandomizedLoopMemory>(SearchData)};

	// ReSharper disable CppRedundantParentheses
	if ((bParentIsSpecialNode && ParentMemory.CurrentChild == BTSpecialChild::NotInitialized) ||
	    (!bParentIsSpecialNode && ParentMemory.CurrentChild != ChildIndex))
	// ReSharper restore CppRedundantParentheses
	{
		Memory.RemainingLoopsCount = FMath::RandRange(MinLoopsCount, MaxLoopsCount);
	}

	Memory.RemainingLoopsCount -= 1;

	if (Memory.RemainingLoopsCount > 0)
	{
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}

#if WITH_EDITOR
FName UFuBTDecorator_RandomizedLoop::GetNodeIconName() const
{
	return TEXT("BTEditor.Graph.BTNode.Decorator.Loop.Icon");
}
#endif
