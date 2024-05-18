#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "FuBTDecorator_HasTag.generated.h"

struct FFuHasTagMemory;

UENUM(BlueprintType)
enum class EFuTagMatchMode : uint8
{
	AnyTag,
	AllTags
};

UCLASS(DisplayName = "Fu Has Tag", Meta = (ShowWorldContextPin))
class FABULOUSAI_API UFuBTDecorator_HasTag : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EFuTagMatchMode MatchMode{EFuTagMatchMode::AnyTag};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTagContainer Tags;

public:
	UFuBTDecorator_HasTag();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) const override;

private:
	void ReInitializeDecoratorMemory(UBehaviorTreeComponent& BehaviorTree, FFuHasTagMemory& Memory);

	void ClearDecoratorMemory(FFuHasTagMemory& Memory);

	EBlackboardNotificationResult Blackboard_OnTargetKeyChanged(const UBlackboardComponent& Blackboard, FBlackboard::FKey Key);

	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 TagCount, TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree);
};
