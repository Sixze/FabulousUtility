#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_EffectTimeListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FFuEffectTimeListenerDelegate, const FGameplayTag&, EffectTag,
                                              float, TimeRemaining, float, Duration, bool, bWaitingForTimeFromServer);

UCLASS(DisplayName = "Fu Effect Time Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_EffectTimeListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer EffectTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bWaitForTimeFromServer;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Time Listener Ability Async")
	FFuEffectTimeListenerDelegate OnEffectStated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Time Listener Ability Async")
	FFuEffectTimeListenerDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectTimeChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Effect Tag") FGameplayTag InEffectTag,
		UPARAM(DisplayName = "Wait For Time From Server") bool bInWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectsTimeChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Effect Tags") FGameplayTagContainer InEffectTags,
		UPARAM(DisplayName = "Wait For Time From Server") bool bInWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectTimeChange(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Effect Tag") FGameplayTag InEffectTag,
		UPARAM(DisplayName = "Wait For Time From Server") bool bInWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectsTimeChange(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Effect Tags") FGameplayTagContainer InEffectTags,
		UPARAM(DisplayName = "Wait For Time From Server") bool bInWaitForTimeFromServer = true);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const;

	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 Count) const;

	void ActiveEffect_OnTimeChanged(FActiveGameplayEffectHandle EffectHandle, float StartTime, float Duration) const;
};
