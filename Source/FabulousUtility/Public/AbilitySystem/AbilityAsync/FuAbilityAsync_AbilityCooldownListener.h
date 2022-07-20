#pragma once

#include "FuAbilityAsync_EffectTimeListener.h"
#include "FuAbilityAsync_AbilityCooldownListener.generated.h"

class UFuAbilitySystemComponent;

UCLASS(DisplayName = "Fu Ability Cooldown Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_AbilityCooldownListener : public UAbilityAsync
{
	GENERATED_BODY()

private:
	FGameplayTagCountContainer EffectTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Input Id", Meta = (AllowPrivateAccess))
	int32 InputId1{-1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Wait For Time From Server", Meta = (AllowPrivateAccess))
	bool bWaitForTimeFromServer1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Cooldown Listener Ability Async")
	FFuEffectTimeChangedDelegate OnEffectStated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Cooldown Listener Ability Async")
	FFuEffectTimeChangedDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldownActor(const AActor* Actor,
	                                                                                int32 InputId,
	                                                                                bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityCooldownListener* FuListenForAbilityCooldown(UFuAbilitySystemComponent* AbilitySystem,
	                                                                           int32 InputId,
	                                                                           bool bWaitForTimeFromServer = true);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void ProcessAbilitySpecificationChange(const FGameplayAbilitySpec& AbilitySpecification, bool bAddedOrRemoved);

	void RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const;

	UFUNCTION()
	void OnAbilityGiven(UFuAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION()
	void OnAbilityRemoved(UFuAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpec& AbilitySpecification);

	void OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
	                                 const FGameplayEffectSpec& EffectSpecification,
	                                 FActiveGameplayEffectHandle EffectHandle) const;

	void OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void OnEffectTimeChanged(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration) const;

	void OnEffectTagChanged(FGameplayTag EffectTag, int32 NewCount) const;
};
