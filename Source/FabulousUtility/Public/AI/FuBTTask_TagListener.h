#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FuBTTask_TagListener.generated.h"

class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EFuTagListenerWaitMode : uint8
{
	WaitForTagAdd,
	WaitForTagRemove,
};

namespace FuTagListenerWaitMode
{
	constexpr bool IsValid(const EFuTagListenerWaitMode WaitMode)
	{
		return WaitMode >= EFuTagListenerWaitMode::WaitForTagAdd && WaitMode <= EFuTagListenerWaitMode::WaitForTagRemove;
	}
}

UCLASS(DisplayName = "Wait for Tag Change", Meta = (ShowWorldContextPin))
class FABULOUSUTILITY_API UFuBTTask_TagListener : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, Category = "Settings")
	EFuTagListenerWaitMode WaitMode{EFuTagListenerWaitMode::WaitForTagRemove};

	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;

public:
	UFuBTTask_TagListener();

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* Property) const override;
#endif

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory) override;

protected:
	virtual void OnTaskFinished(UBehaviorTreeComponent& BehaviourTree, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	void OnTagChanged(FGameplayTag ThisTag, int32 NewCount) const;
};
