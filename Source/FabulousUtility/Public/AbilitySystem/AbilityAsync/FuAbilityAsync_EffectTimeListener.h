#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_EffectTimeListener.generated.h"

class UFuAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FFuEffectTimeChangedDelegate, const FGameplayTag&, EffectTag,
                                              float, TimeRemaining, float, Duration, bool, bWaitingForTimeFromServer);

UCLASS(DisplayName = "Fu Effect Time Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_EffectTimeListener : public UAbilityAsync
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Effect Tags", Meta = (AllowPrivateAccess))
	FGameplayTagContainer EffectTags1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Wait For Time From Server", Meta = (AllowPrivateAccess))
	bool bWaitForTimeFromServer1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Time Listene Ability Async")
	FFuEffectTimeChangedDelegate OnEffectStated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Time Listener Ability Async")
	FFuEffectTimeChangedDelegate OnEffectEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectTimeChangeActor(const AActor* Actor,
	                                                                            FGameplayTag EffectTag,
	                                                                            bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectsTimeChangeActor(const AActor* Actor,
	                                                                             FGameplayTagContainer EffectTags,
	                                                                             bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectTimeChange(UFuAbilitySystemComponent* AbilitySystem,
	                                                                       FGameplayTag EffectTag,
	                                                                       bool bWaitForTimeFromServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectTimeListener* FuListenForEffectsTimeChange(UFuAbilitySystemComponent* AbilitySystem,
	                                                                        FGameplayTagContainer EffectTags,
	                                                                        bool bWaitForTimeFromServer = true);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const;

	void OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
	                                 const FGameplayEffectSpec& EffectSpecification,
	                                 FActiveGameplayEffectHandle EffectHandle) const;

	void OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void OnEffectTimeChanged(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration) const;

	void OnEffectTagChanged(FGameplayTag EffectTag, int32 NewCount) const;
};
