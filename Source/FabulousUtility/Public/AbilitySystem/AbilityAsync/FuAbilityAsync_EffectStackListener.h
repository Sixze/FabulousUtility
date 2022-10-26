#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "AbilitySystem/AbilityTasks/FuAbilityTask_EffectStackListener.h"
#include "FuAbilityAsync_EffectStackListener.generated.h"

class UFuAbilitySystemComponent;

UCLASS(DisplayName = "Fu Effect Stack Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_EffectStackListener : public UAbilityAsync
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Effect Class", Meta = (AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> EffectClass1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Stack Listener Ability Async")
	FFuEffectStackChangedDelegate OnStackChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectStackListener* FuListenForEffectStackChangeActor(const AActor* Actor,
	                                                                              TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectStackListener* FuListenForEffectStackChange(UFuAbilitySystemComponent* AbilitySystem,
	                                                                         TSubclassOf<UGameplayEffect> EffectClass);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
	                                 const FGameplayEffectSpec& EffectSpecification,
	                                 FActiveGameplayEffectHandle EffectHandle) const;

	void OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void OnEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewCount, int32 PreviousCount) const;
};
