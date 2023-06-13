#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityAsync/FuAbilityAsync_TagListener.h"
#include "FuAbilityTask_TagListener.generated.h"

UCLASS(DisplayName = "Fu Tag Listener Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_TagListener : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer Tags;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Tag Listener Ability Task")
	FFuTagListenerDelegate OnTagAdded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Tag Listener Ability Task")
	FFuTagListenerDelegate OnTagRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TagListener* FuWaitForTagChange(UGameplayAbility* OwningAbility,
	                                                      UPARAM(DisplayName = "Tag") FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TagListener* FuWaitForTagsChange(UGameplayAbility* OwningAbility,
	                                                       UPARAM(DisplayName = "Tags") FGameplayTagContainer InTags);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 Count) const;
};
