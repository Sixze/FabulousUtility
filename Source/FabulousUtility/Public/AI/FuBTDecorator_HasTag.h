#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "FuBTDecorator_HasTag.generated.h"

class UAbilitySystemComponent;

UCLASS(DisplayName = "Has Tag", Meta = (ShowWorldContextPin))
class FABULOUSUTILITY_API UFuBTDecorator_HasTag : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	EGameplayContainerMatchType TagsMatchMode{EGameplayContainerMatchType::All};

	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTagContainer Tags;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;

public:
	UFuBTDecorator_HasTag();

	virtual FString GetStaticDescription() const override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) const override;

private:
	void OnTagChanged(FGameplayTag ThisTag, int32 NewCount) const;
};
