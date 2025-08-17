#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "FuBTDecorator_RandomizedLoop.generated.h"

// Randomized loop decorator node.
// A decorator node that bases its condition on whether its loop counter has been exceeded.
UCLASS(DisplayName = "Fu Randomized Loop", HideCategories = ("Condition"), Meta = (ShowWorldContextPin))
class FABULOUSAI_API UFuBTDecorator_RandomizedLoop : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 1))
	FInt32Interval LoopsCount{1, 3};

public:
	UFuBTDecorator_RandomizedLoop();

	virtual void InitializeMemory(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;

	virtual void CleanupMemory(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory,
	                                   EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
};
