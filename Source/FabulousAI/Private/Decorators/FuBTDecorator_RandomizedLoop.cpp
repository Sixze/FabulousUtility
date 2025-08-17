#include "Decorators/FuBTDecorator_RandomizedLoop.h"

#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTDecorator_RandomizedLoop)

struct FFuRandomizedLoopMemory
{
	int32 RemainingLoopsCount{0};
};

UFuBTDecorator_RandomizedLoop::UFuBTDecorator_RandomizedLoop()
{
	NodeName = TEXTVIEW("Fu Randomized Loop");

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UFuBTDecorator_RandomizedLoop::InitializeMemory(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory,
                                                     const EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FFuRandomizedLoopMemory>(NodeMemory, InitType);
}

void UFuBTDecorator_RandomizedLoop::CleanupMemory(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory,
                                                  const EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FFuRandomizedLoopMemory>(NodeMemory, CleanupType);
}

void UFuBTDecorator_RandomizedLoop::DescribeRuntimeValues(const UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory,
                                                          const EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(BehaviorTree, NodeMemory, Verbosity, Values);

	const auto& Memory{*CastInstanceNodeMemory<FFuRandomizedLoopMemory>(NodeMemory)};

	TStringBuilder<32> RuntimeValuesBuilder{InPlace, TEXTVIEW("Loops remaining: "), Memory.RemainingLoopsCount};

	Values.Emplace(FString{RuntimeValuesBuilder});
}

uint16 UFuBTDecorator_RandomizedLoop::GetInstanceMemorySize() const
{
	return sizeof(FFuRandomizedLoopMemory);
}

FString UFuBTDecorator_RandomizedLoop::GetStaticDescription() const
{
	TStringBuilder<64> DescriptionBuilder{
		InPlace, TEXTVIEW("Randomized Loop: "), LoopsCount.Min, TEXT('-'), LoopsCount.Max, TEXTVIEW(" loops")
	};

	return FString{DescriptionBuilder};
}

void UFuBTDecorator_RandomizedLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	const auto& ParentMemory{*GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData)};
	const auto bParentIsSpecialNode{GetParentNode()->IsA<UBTComposite_SimpleParallel>()};

	auto& Memory{*GetNodeMemory<FFuRandomizedLoopMemory>(SearchData)};

	if ((bParentIsSpecialNode && ParentMemory.CurrentChild == BTSpecialChild::NotInitialized) ||
	    (!bParentIsSpecialNode && static_cast<uint8>(ParentMemory.CurrentChild) != ChildIndex))
	{
		Memory.RemainingLoopsCount = FMath::RandRange(LoopsCount.Min, LoopsCount.Max);
	}

	Memory.RemainingLoopsCount -= 1;

	if (Memory.RemainingLoopsCount > 0)
	{
		GetParentNode()->SetChildOverride(SearchData, static_cast<int8>(ChildIndex));
	}
}

#if WITH_EDITOR
FName UFuBTDecorator_RandomizedLoop::GetNodeIconName() const
{
	return FName{TEXTVIEW("BTEditor.Graph.BTNode.Decorator.Loop.Icon")};
}
#endif
