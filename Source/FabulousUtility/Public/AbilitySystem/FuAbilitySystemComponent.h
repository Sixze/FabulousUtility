#pragma once

#include "AbilitySystemComponent.h"
#include "FuAbilitySystemComponent.generated.h"

class UFuAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuAbilityAddedOrRemovedDelegate, UFuAbilitySystemComponent*, AbilitySystem,
                                             const FGameplayAbilitySpec&, AbilitySpecification);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuAbilityActivatedDelegate, FGameplayAbilitySpecHandle, AbilityHandle,
                                             UGameplayAbility*, Ability);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFuAbilityFailedDelegate, FGameplayAbilitySpecHandle, AbilityHandle,
                                               UGameplayAbility*, Ability, const FGameplayTagContainer&, FailureTags);

UCLASS()
class FABULOUSUTILITY_API UFuAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

private:
	FGameplayTagCountContainer BlockedAbilityWithoutTags;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability System")
	FFuAbilityAddedOrRemovedDelegate OnAbilityGiven;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability System")
	FFuAbilityAddedOrRemovedDelegate OnAbilityRemoved;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability System")
	FFuAbilityActivatedDelegate OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability System")
	FFuAbilityFailedDelegate OnAbilityFailed;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetFuAbilitySystem(const UObject* Object, UFuAbilitySystemComponent*& AbilitySystem, bool bAllowFindComponent = true);

	virtual void NotifyAbilityActivated(FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability) override;

	virtual void NotifyAbilityFailed(FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability,
	                                 const FGameplayTagContainer& FailureTags) override;

protected:
	virtual bool ShouldDoServerAbilityRPCBatch() const override;

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpecification) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpecification) override;

	virtual bool AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const override;

	virtual void AbilityLocalInputPressed(int32 InputId) override;

	// Effect

public:
	const FActiveGameplayEffectsContainer* GetActiveEffects() const;

	FActiveGameplayEffectsContainer* GetActiveEffectsMutable();

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectQuery"))
	bool HasAnyActiveEffectsByQuery(const FGameplayEffectQuery& EffectQuery) const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectQuery"))
	void GetActiveEffectsByQuery(const FGameplayEffectQuery& EffectQuery, TArray<FActiveGameplayEffect>& ActiveEffects) const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectTag"))
	bool HasAnyActiveEffectsWithTag(const FGameplayTag& EffectTag, bool bIgnoreInhibitedEffects = false) const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectTags"))
	bool HasAnyActiveEffectsWithAnyTags(const FGameplayTagContainer& EffectTags, bool bIgnoreInhibitedEffects = false) const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectTag"))
	int32 GetEffectsCountWithTag(const FGameplayTag& EffectTag, bool bIgnoreInhibitedEffects = false) const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectTags"))
	int32 GetEffectsCountWithAnyTags(const FGameplayTagContainer& EffectTags, bool bIgnoreInhibitedEffects = false) const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System")
	int32 GetEffectStacksCountByClass(TSubclassOf<UGameplayEffect> EffectClass) const;

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System")
	void RecalculateEffectModifiers(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System")
	void SetEffectDuration(FActiveGameplayEffectHandle EffectHandle, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System")
	void SetEffectTimeRemaining(FActiveGameplayEffectHandle EffectHandle, float TimeRemaining);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System")
	void IncreaseEffectTimeRemaining(FActiveGameplayEffectHandle EffectHandle, float AdditionalTimeRemaining);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "EffectTag"))
	void GetEffectTimeRemainingAndDurationByTag(const FGameplayTag& EffectTag, float& TimeRemaining, float& Duration) const;

	const FActiveGameplayEffect* GetActiveEffectTimeRemainingAndDurationByTag(const FGameplayTag& EffectTag, float& TimeRemaining,
	                                                                          float& Duration) const;

	// Ability

	void BlockAbilitiesWithoutTags(const FGameplayTagContainer& Tags);

	void UnBlockAbilitiesWithoutTags(const FGameplayTagContainer& Tags);
};

inline const FActiveGameplayEffectsContainer* UFuAbilitySystemComponent::GetActiveEffects() const
{
	return &ActiveGameplayEffects;
}

inline FActiveGameplayEffectsContainer* UFuAbilitySystemComponent::GetActiveEffectsMutable()
{
	return &ActiveGameplayEffects;
}
