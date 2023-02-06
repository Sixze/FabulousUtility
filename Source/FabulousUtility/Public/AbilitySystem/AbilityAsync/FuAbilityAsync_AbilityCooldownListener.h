#pragma once

#include "FuAbilityAsync_EffectTimeListener.h"
#include "FuAbilityAsync_AbilityCooldownListener.generated.h"

class UFuAbilitySystemComponent;

UCLASS(DisplayName = "Fu Ability Cooldown Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_AbilityCooldownListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	FGameplayTagCountContainer EffectTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Ability Tags")
	FGameplayTagContainer AbilityTags1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Input Id")
	int32 InputId1{-1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Wait For Time From Server")
	bool bWaitForTimeFromServer1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Cooldown Listener Ability Async")
	FFuEffectTimeListenerDelegate OnEffectStated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Cooldown Listener Ability Async")
	FFuEffectTimeListenerDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownByAbilityTagOnActor(
		const AActor* Actor, FGameplayTag AbilityTag, bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownByAbilityTagsOnActor(
		const AActor* Actor, FGameplayTagContainer AbilityTags, bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownByInputIdOnActor(
		const AActor* Actor, int32 InputId, bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownByAbilityTag(
		UFuAbilitySystemComponent* AbilitySystem, FGameplayTag AbilityTag, bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownByAbilityTags(
		UFuAbilitySystemComponent* AbilitySystem, FGameplayTagContainer AbilityTags, bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownByInputId(
		UFuAbilitySystemComponent* AbilitySystem, int32 InputId, bool bWaitForTimeFromServer = true);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void ProcessAbilitySpecificationChange(const FGameplayAbilitySpec& AbilitySpecification, bool bAddedOrRemoved);

	void RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const;

	void AbilitySystem_OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpecification);

	void AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification);

	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 Count) const;

	void ActiveEffect_OnTimeChanged(FActiveGameplayEffectHandle EffectHandle, float StartTime, float Duration) const;
};
