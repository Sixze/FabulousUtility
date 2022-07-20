#pragma once

#include "AbilitySystem/FuAbilitySystemComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FuBTTask_ActivateAbility.generated.h"

UCLASS(Meta = (ShowWorldContextPin))
class FABULOUSUTILITY_API UFuBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bWaitForAbilityEnd{true};

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bCancelAbilityOnAbort;

	UPROPERTY(Transient)
	TWeakObjectPtr<UFuAbilitySystemComponent> AbilitySystem;

	UPROPERTY(Transient)
	TSet<FGameplayAbilitySpecHandle> ActiveAbilityHandles;

	UPROPERTY(Transient)
	bool bAnyAbilitySuccessfullyEnded;

public:
	UFuBTTask_ActivateAbility();

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	UFUNCTION()
	void OnAbilityActivated(FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability);

	void OnAbilityEnded(const FAbilityEndedData& EndedData);
};
