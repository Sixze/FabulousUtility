#pragma once

#include "Abilities/GameplayAbility.h"
#include "FuGameplayAbility.generated.h"

UCLASS(AutoExpandCategories = ("Settings|Fu Gameplay Ability", "State|Fu Gameplay Ability"))
class FABULOUSABILITYSYSTEM_API UFuGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	FGameplayTagContainer RemoveAbilitiesWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	FGameplayTagContainer CancelAbilitiesWithoutTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	FGameplayTagContainer BlockAbilitiesWithoutTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	bool bAutoActivateWhenGiven;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	bool bAllowActivationByInput{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Instanced)
	TObjectPtr<UObject> UserData;

	bool bCheckCostBlueprintImplemented{false};

public:
	UFuGameplayAbility();

	virtual void SetShouldBlockOtherAbilities(bool bShouldBlockAbilities) override;

	virtual bool CheckCost(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayTagContainer* FailureTags = nullptr) const override;

	virtual void ApplyCost(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& AbilitySpecification) override;

protected:
	virtual void PreActivate(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
	                         FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
	                        FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bCanceled) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Fu Gameplay Ability", DisplayName = "Check Cost")
	bool CheckCostBlueprint(const FGameplayAbilityActorInfo& ActorInfo, FGameplayAbilitySpecHandle AbilityHandle) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Fu Gameplay Ability", DisplayName = "Apply Cost", Meta = (ForceAsFunction))
	void ApplyCostBlueprint(const FGameplayAbilityActorInfo& ActorInfo, FGameplayAbilitySpecHandle AbilityHandle,
	                        const FGameplayAbilityActivationInfo& ActivationInfo) const;

public:
	bool IsActivationByInputAllowed() const;

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool TryBatchRpcActivateAbility(FGameplayAbilitySpecHandle AbilityHandle, bool bEndAbilityImmediately);

	void BatchRpcEndAbility();
};

inline bool UFuGameplayAbility::IsActivationByInputAllowed() const
{
	return bAllowActivationByInput;
}
