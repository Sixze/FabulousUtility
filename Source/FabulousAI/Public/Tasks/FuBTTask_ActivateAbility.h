#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FuBTTask_ActivateAbility.generated.h"

struct FAbilityEndedData;

UCLASS(DisplayName = "Fu Activate Ability", Meta = (ShowWorldContextPin))
class FABULOUSAI_API UFuBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	uint8 bWaitForAbilityEnd : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (EditCondition = "bWaitForAbilityEnd"))
	uint8 bCancelAbilityOnAbort : 1 {false};

public:
	UFuBTTask_ActivateAbility();

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	void AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData, TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree);
};
