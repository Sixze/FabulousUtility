#pragma once

#include "Abilities/GameplayAbility.h"
#include "FuGameplayAbility.generated.h"

UCLASS(AutoExpandCategories = ("Settings|Fu Gameplay Ability", "State|Fu Gameplay Ability"))
class FABULOUSABILITYSYSTEM_API UFuGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	FGameplayTagContainer RemoveAbilitiesWithAnyTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	FGameplayTagContainer CancelAbilitiesWithoutAllTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	FGameplayTagContainer BlockAbilitiesWithoutAllTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	uint8 bAutoActivateWhenGiven : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability")
	uint8 bAllowActivationByInput : 1 {true};

	// If checked, RPC batching will be used during activation of this ability by input.
	// See https://github.com/tranek/GASDocumentation#concepts-ga-batching
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability",
		Meta = (EditCondition = "bAllowActivationByInput"))
	uint8 bBatchActivationByInput : 1 {false};

	uint8 bCheckCostBlueprintImplemented : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Instanced)
	TObjectPtr<UObject> UserData;

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

	bool CanBatchActivationByInput() const;

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Gameplay Ability", Meta = (ReturnDisplayName = "Success"))
	bool BatchRpcActivateAbility(FGameplayAbilitySpecHandle AbilityHandle, bool bEndAbilityImmediately);

	void BatchRpcEndAbility();
};

inline bool UFuGameplayAbility::IsActivationByInputAllowed() const
{
	return bAllowActivationByInput;
}

inline bool UFuGameplayAbility::CanBatchActivationByInput() const
{
	return bBatchActivationByInput;
}
