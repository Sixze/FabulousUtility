#pragma once

#include "Abilities/GameplayAbility.h"
#include "FuGameplayAbility.generated.h"

UCLASS(AutoExpandCategories = ("Settings|Fu Gameplay Ability", "State|Fu Gameplay Ability"))
class FABULOUSUTILITY_API UFuGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Meta = (AllowPrivateAccess))
	FGameplayTagContainer RemoveAbilitiesWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Meta = (AllowPrivateAccess))
	FGameplayTagContainer CancelAbilitiesWithoutTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Meta = (AllowPrivateAccess))
	FGameplayTagContainer BlockAbilitiesWithoutTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Meta = (AllowPrivateAccess))
	bool bAutoActivateWhenGiven;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Meta = (AllowPrivateAccess))
	bool bAllowActivationByInput{true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Ability", Instanced, Meta = (AllowPrivateAccess))
	TObjectPtr<UObject> UserData;

public:
	UFuGameplayAbility();

	virtual void SetShouldBlockOtherAbilities(bool bShouldBlockAbilities) override;

	virtual bool CheckCost(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ApplyCost(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& AbilitySpecification) override;

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle AbilityHandle,
	                         const FGameplayAbilityActorInfo* ActorInfo,
	                         const FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(FGameplayAbilitySpecHandle AbilityHandle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility, bool bCancelled) override;
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Fabulous Utility|Fu Gameplay Ability", DisplayName = "Check Cost")
	bool CheckCostBlueprint(const FGameplayAbilityActorInfo& ActorInfo, FGameplayAbilitySpecHandle AbilityHandle) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Fabulous Utility|Fu Gameplay Ability", DisplayName = "Apply Cost")
	void ApplyCostBlueprint(const FGameplayAbilityActorInfo& ActorInfo, FGameplayAbilitySpecHandle AbilityHandle,
	                        FGameplayAbilityActivationInfo ActivationInfo) const;

public:
	bool IsActivationByInputAllowed() const;

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Ability")
	bool TryBatchRpcActivateAbility(FGameplayAbilitySpecHandle AbilityHandle, bool bEndAbilityImmediately);

	void BatchRpcEndAbility();
};

inline bool UFuGameplayAbility::IsActivationByInputAllowed() const
{
	return bAllowActivationByInput;
}
