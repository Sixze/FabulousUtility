#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "FuBTDecorator_CanActivateAbility.generated.h"

struct FGameplayAbilitySpec;
class UFuAbilitySystemComponent;

UCLASS(DisplayName = "Fu Can Activate Ability", Meta = (ShowWorldContextPin))
class FABULOUSUTILITY_API UFuBTDecorator_CanActivateAbility : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTagContainer AbilityTags;

public:
	UFuBTDecorator_CanActivateAbility();

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, float DeltaTime) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) const override;

private:
	void OnAbilityGiven(UFuAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpec& AbilitySpecification,
	                    TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree);

	void OnAbilityRemoved(UFuAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpec& AbilitySpecification,
	                      TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree);
};
