#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_EffectTimeListener.generated.h"

class UFuAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FFuEffectTimeListenerDelegate, const FGameplayTag&, EffectTag,
                                              float, TimeRemaining, float, Duration, bool, bWaitingForTimeFromServer);

UCLASS(DisplayName = "Fu Effect Time Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_EffectTimeListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Effect Tags")
	FGameplayTagContainer EffectTags1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Wait For Time From Server")
	bool bWaitForTimeFromServer1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Time Listene Ability Async")
	FFuEffectTimeListenerDelegate OnEffectStated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Time Listener Ability Async")
	FFuEffectTimeListenerDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectTimeChangeOnActor(const AActor* Actor,
	                                                                              FGameplayTag EffectTag,
	                                                                              bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectsTimeChangeOnActor(const AActor* Actor,
	                                                                               FGameplayTagContainer EffectTags,
	                                                                               bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectTimeChange(UFuAbilitySystemComponent* AbilitySystem,
	                                                                       FGameplayTag EffectTag,
	                                                                       bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectsTimeChange(UFuAbilitySystemComponent* AbilitySystem,
	                                                                        FGameplayTagContainer EffectTags,
	                                                                        bool bWaitForTimeFromServer = true);

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
