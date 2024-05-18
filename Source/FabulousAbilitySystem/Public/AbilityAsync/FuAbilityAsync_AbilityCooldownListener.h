#pragma once

#include "FuAbilityAsync_EffectTimeListener.h"
#include "FuAbilityAsync_AbilityCooldownListener.generated.h"

UCLASS(DisplayName = "Fu Ability Cooldown Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_AbilityCooldownListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	FGameplayTagCountContainer EffectTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer AbilityTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	int32 InputId{-1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bPredictedTimeAllowed : 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Ability Cooldown Listener")
	FFuEffectTimeListenerDelegate OnEffectStated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Ability Cooldown Listener")
	FFuEffectTimeListenerDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* ListenForAbilityCooldownByAbilityTagOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Ability Tag") FGameplayTag InAbilityTag,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* ListenForAbilityCooldownByAbilityTagsOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Ability Tags") FGameplayTagContainer InAbilityTags,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* ListenForAbilityCooldownByInputIdOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Input Id") int32 InInputId,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* ListenForAbilityCooldownByAbilityTag(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Ability Tag") FGameplayTag InAbilityTag,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* ListenForAbilityCooldownByAbilityTags(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Ability Tags") FGameplayTagContainer InAbilityTags,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* ListenForAbilityCooldownByInputId(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Input Id") int32 InInputId,
		bool bAllowPredictedTime = false);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void ProcessAbilitySpecificationChange(const FGameplayAbilitySpec& AbilitySpecification, bool bAddedOrRemoved);

	void BroadcastEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const;

	void AbilitySystem_OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpecification);

	void AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification);

	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 TagCount) const;

	void ActiveEffect_OnTimeChanged(FActiveGameplayEffectHandle EffectHandle, float StartTime, float Duration) const;
};
