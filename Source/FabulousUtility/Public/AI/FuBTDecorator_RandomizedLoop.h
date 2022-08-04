#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "FuBTDecorator_RandomizedLoop.generated.h"

// Randomized loop decorator node.
// A decorator node that bases its condition on whether its loop counter has been exceeded.
UCLASS(HideCategories = ("Condition"), Meta = (ShowWorldContextPin))
class FABULOUSUTILITY_API UFuBTDecorator_RandomizedLoop : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings", Meta = (ClampMin = 1))
	int32 MinLoopsCount{1};

	UPROPERTY(EditAnywhere, Category = "Settings", Meta = (ClampMin = 1))
	int32 MaxLoopsCount{3};

public:
	UFuBTDecorator_RandomizedLoop();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory,
	                                   EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
};
