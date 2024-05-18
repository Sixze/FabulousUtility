#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FuBTTask_WaitForTagChange.generated.h"

UENUM(BlueprintType)
enum class EFuTagWaitMode : uint8
{
	WaitForTagAdd,
	WaitForTagRemove,
};

namespace FuTagListenerWaitMode
{
	FABULOUSAI_API constexpr bool IsValid(const EFuTagWaitMode WaitMode)
	{
		return WaitMode >= EFuTagWaitMode::WaitForTagAdd && WaitMode <= EFuTagWaitMode::WaitForTagRemove;
	}
}

UCLASS(DisplayName = "Fu Wait for Tag Change", Meta = (ShowWorldContextPin))
class FABULOUSAI_API UFuBTTask_WaitForTagChange : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EFuTagWaitMode WaitMode{EFuTagWaitMode::WaitForTagRemove};

public:
	UFuBTTask_WaitForTagChange();

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* Property) const override;
#endif

	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory) override;

protected:
	virtual void OnTaskFinished(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	void AbilitySystem_OnTagChanged(FGameplayTag OtherTag, int32 TagCount, TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTree) const;
};
