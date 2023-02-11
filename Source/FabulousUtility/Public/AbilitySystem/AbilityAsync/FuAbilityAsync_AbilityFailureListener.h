#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_AbilityFailureListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuAbilityFailureListenerDelegate, FGameplayAbilitySpecHandle, AbilityHandle,
                                             const FGameplayTagContainer&, FailureTags);

UCLASS(DisplayName = "Fu Ability Failure Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_AbilityFailureListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Ability Tags")
	FGameplayTagContainer AbilityTags1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Failure Tags")
	FGameplayTagContainer FailureTags1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Failure Listener Ability Async")
	FFuAbilityFailureListenerDelegate OnAbilityFailed;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilityFailureOnActor(const AActor* Actor,
	                                                                                FGameplayTag AbilityTag,
	                                                                                FGameplayTagContainer FailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilitiesFailureOnActor(const AActor* Actor,
	                                                                                  FGameplayTagContainer AbilityTags,
	                                                                                  FGameplayTagContainer FailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilityFailure(UFuAbilitySystemComponent* AbilitySystem,
	                                                                         FGameplayTag AbilityTag,
	                                                                         FGameplayTagContainer FailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* FuListenForAbilitiesFailure(UFuAbilitySystemComponent* AbilitySystem,
	                                                                           FGameplayTagContainer AbilityTags,
	                                                                           FGameplayTagContainer FailureTags);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnAbilityFailed(FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability,
	                                   const FGameplayTagContainer& FailureTags) const;
};
