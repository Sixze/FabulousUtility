#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "FuBTDecorator_ReceiveGameplayEvent.generated.h"

struct FFuReceiveGameplayEventMemory;
struct FGameplayEventData;

UCLASS(DisplayName = "Fu Receive Gameplay Event", HideCategories = ("Condition"), Meta = (ShowWorldContextPin))
class FABULOUSAI_API UFuBTDecorator_ReceiveGameplayEvent : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTagContainer EventTags;

	// If checked and FlowAbortMode is set to LowerPriority or Self, then the execution
	// flow will only be able to pass through this node upon receiving a gameplay event.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	uint8 bAllowEntryInNonSelfFlowAbortMode : 1 {true};

public:
	UFuBTDecorator_ReceiveGameplayEvent();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) const override;

private:
	void ReInitializeDecoratorMemory(UBehaviorTreeComponent& BehaviorTree, FFuReceiveGameplayEventMemory& Memory);

	void ClearDecoratorMemory(FFuReceiveGameplayEventMemory& Memory);

	EBlackboardNotificationResult Blackboard_OnTargetKeyChanged(const UBlackboardComponent& Blackboard, FBlackboard::FKey Key);

	void AbilitySystem_OnEventReceived(const FGameplayEventData* EventData, TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree);
};
