#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_EffectTimeListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FFuEffectTimeListenerDelegate, const FGameplayTag&, EffectTag,
                                              float, TimeRemaining, float, Duration, bool, bPredictedTime);

UCLASS(DisplayName = "Fu Effect Time Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_EffectTimeListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer EffectTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bPredictedTimeAllowed : 1 {false};

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Effect Time Listener")
	FFuEffectTimeListenerDelegate OnEffectStarted;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Effect Time Listener")
	FFuEffectTimeListenerDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* ListenForEffectTimeChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Effect Tag") FGameplayTag InEffectTag,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* ListenForEffectsTimeChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Effect Tags") FGameplayTagContainer InEffectTags,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* ListenForEffectTimeChange(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Effect Tag") FGameplayTag InEffectTag,
		bool bAllowPredictedTime = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* ListenForEffectsTimeChange(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Effect Tags") FGameplayTagContainer InEffectTags,
		bool bAllowPredictedTime = false);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void BroadcastEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const;

	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 TagCount) const;

	void ActiveEffect_OnTimeChanged(FActiveGameplayEffectHandle EffectHandle, float StartTime, float Duration) const;
};
