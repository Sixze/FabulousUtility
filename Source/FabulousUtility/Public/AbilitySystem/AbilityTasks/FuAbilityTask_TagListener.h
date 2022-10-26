#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/AbilityAsync/FuAbilityAsync_TagListener.h"
#include "FuAbilityTask_TagListener.generated.h"

UCLASS(DisplayName = "Fu Tag Listener Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_TagListener : public UAbilityTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Tags", Meta = (AllowPrivateAccess))
	FGameplayTagContainer Tags1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Tag Listener Ability Task")
	FFuTagChangedDelegate OnTagAdded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Tag Listener Ability Task")
	FFuTagChangedDelegate OnTagRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TagListener* FuWaitForTagChange(UGameplayAbility* OwningAbility, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TagListener* FuWaitForTagsChange(UGameplayAbility* OwningAbility, FGameplayTagContainer Tags);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void OnTagChanged(FGameplayTag Tag, int32 NewCount) const;
};
