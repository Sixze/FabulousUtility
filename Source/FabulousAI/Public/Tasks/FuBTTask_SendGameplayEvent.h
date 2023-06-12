#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FuBTTask_SendGameplayEvent.generated.h"

UCLASS(DisplayName = "Fu Send Gameplay Event", Meta = (ShowWorldContextPin))
class FABULOUSAI_API UFuBTTask_SendGameplayEvent : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag EventTag;

public:
	UFuBTTask_SendGameplayEvent();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;
};
