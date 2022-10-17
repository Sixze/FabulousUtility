#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FuBTTask_GameplayEvent.generated.h"

UCLASS(DisplayName = "Fu Send Gameplay Event", Meta = (ShowWorldContextPin))
class FABULOUSUTILITY_API UFuBTTask_GameplayEvent : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTag EventTag;

public:
	UFuBTTask_GameplayEvent();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;
};
