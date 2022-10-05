#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_AbilityFailureListener.generated.h"

class UFuAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuAbilityFailureDelegate, FGameplayAbilitySpecHandle, AbilityHandle,
                                             const FGameplayTagContainer&, FailureTags);

UCLASS(DisplayName = "Fu Ability Failure Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_AbilityFailureListener : public UAbilityAsync
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Ability Tags", Meta = (AllowPrivateAccess))
	FGameplayTagContainer AbilityTags1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Failure Tags", Meta = (AllowPrivateAccess))
	FGameplayTagContainer FailureTags1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Failure Listener Ability Async")
	FFuAbilityFailureDelegate OnAbilityFailed;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilityFailureActor(const AActor* Actor,
	                                                                              FGameplayTag AbilityTag,
	                                                                              FGameplayTagContainer FailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilitiesFailureActor(const AActor* Actor,
	                                                                                FGameplayTagContainer AbilityTags,
	                                                                                FGameplayTagContainer FailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilityFailure(
		UFuAbilitySystemComponent* AbilitySystem, FGameplayTag AbilityTag, FGameplayTagContainer FailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilitiesFailure(
		UFuAbilitySystemComponent* AbilitySystem, FGameplayTagContainer AbilityTags, FGameplayTagContainer FailureTags);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnAbilityFailed(FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability,
	                                   const FGameplayTagContainer& FailureTags) const;
};
