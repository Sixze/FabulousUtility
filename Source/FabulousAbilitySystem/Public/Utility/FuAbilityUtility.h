#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilityUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilityUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ReturnDisplayName = "Cooldown Tags"))
	static const FGameplayTagContainer& GetCooldownTags(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ReturnDisplayName = "Value"))
	static bool IsActive(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsActive(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ReturnDisplayName = "Ability Handle"))
	static FGameplayAbilitySpecHandle GetAbilityHandle(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ReturnDisplayName = "Input Id"))
	static uint8 GetInputId(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ReturnDisplayName = "Value"))
	static bool IsInputPressed(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsInputPressed(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", AutoCreateRefTerm = "ParentTag", ReturnDisplayName = "Descendant Tag"))
	static FGameplayTag GetFirstDescendantAbilityTag(const UGameplayAbility* Ability, const FGameplayTag& ParentTag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (AutoCreateRefTerm = "ParentTag", ReturnDisplayName = "Descendant Tag"))
	static FGameplayTag GetFirstDescendantAbilityTagByHandle(UAbilitySystemComponent* AbilitySystem,
	                                                         FGameplayAbilitySpecHandle AbilityHandle,
	                                                         const FGameplayTag& ParentTag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ReturnDisplayName = "Effect Handle"))
	static FActiveGameplayEffectHandle GetEffectHandleFromGrantedAbility(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryCommitAbility(UGameplayAbility* Ability, bool bCancelOnFailure = true);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ReturnDisplayName = "Ability Specifications"))
	static const TArray<FGameplayAbilitySpec>& GetActivatableAbilities(const UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ReturnDisplayName = "Ability"))
	static const UGameplayAbility* GetPrimaryAbilityInstance(UAbilitySystemComponent* AbilitySystem,
	                                                         FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ReturnDisplayName = "Value"))
	static bool IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Value"))
	static bool HasAbilityWithTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Value"))
	static bool CanActivateAbilityByTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Tag"))
	static bool SwitchCanActivateAbilityByTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ReturnDisplayName = "Value"))
	static bool CanActivateAbilityByClass(UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchCanActivateAbilityByClass(UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ReturnDisplayName = "Value"))
	static bool CanActivateAbilityByHandle(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchCanActivateAbilityByHandle(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool TryBatchRpcActivateAbility(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle,
	                                       bool bEndAbilityImmediately);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility")
	static void CancelAbilityByHandle(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tags"))
	static void CancelAbilitiesWithAnyTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
	                                      UGameplayAbility* IgnoreAbility = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tags"))
	static void CancelAbilitiesWithoutAnyTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
	                                         UGameplayAbility* IgnoreAbility = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "Tags"))
	static void RemoveAbilitiesWithAnyTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
	                                       FGameplayAbilitySpecHandle IgnoreAbilityHandle);
};

inline const FGameplayTagContainer& UFuAbilityUtility::GetCooldownTags(const TSubclassOf<UGameplayAbility> AbilityClass)
{
	const auto* Tags{FU_ENSURE(IsValid(AbilityClass)) ? AbilityClass.GetDefaultObject()->GetCooldownTags() : nullptr};

	static const FGameplayTagContainer None;
	check(None.IsEmpty())

	return Tags != nullptr ? *Tags : None;
}

inline bool UFuAbilityUtility::IsActive(const UGameplayAbility* Ability)
{
	return FU_ENSURE(IsValid(Ability)) && Ability->IsActive();
}

inline bool UFuAbilityUtility::SwitchIsActive(const UGameplayAbility* Ability)
{
	return IsActive(Ability);
}

inline FGameplayAbilitySpecHandle UFuAbilityUtility::GetAbilityHandle(const UGameplayAbility* Ability)
{
	return FU_ENSURE(IsValid(Ability)) ? Ability->GetCurrentAbilitySpecHandle() : FGameplayAbilitySpecHandle{};
}

inline uint8 UFuAbilityUtility::GetInputId(const UGameplayAbility* Ability)
{
	const auto* AbilitySpecification{IsValid(Ability) ? Ability->GetCurrentAbilitySpec() : nullptr};

	return FU_ENSURE(AbilitySpecification != nullptr) ? static_cast<uint8>(AbilitySpecification->InputID) : -1;
}

inline bool UFuAbilityUtility::IsInputPressed(const UGameplayAbility* Ability)
{
	const auto* AbilitySpecification{IsValid(Ability) ? Ability->GetCurrentAbilitySpec() : nullptr};

	return FU_ENSURE(AbilitySpecification != nullptr) && AbilitySpecification->InputPressed;
}

inline bool UFuAbilityUtility::SwitchIsInputPressed(const UGameplayAbility* Ability)
{
	return IsInputPressed(Ability);
}

inline FActiveGameplayEffectHandle UFuAbilityUtility::GetEffectHandleFromGrantedAbility(const UGameplayAbility* Ability)
{
	const auto* AbilitySpecification{IsValid(Ability) ? Ability->GetCurrentAbilitySpec() : nullptr};

	return FU_ENSURE(AbilitySpecification != nullptr) ? AbilitySpecification->GameplayEffectHandle : FActiveGameplayEffectHandle{};
}

inline const TArray<FGameplayAbilitySpec>& UFuAbilityUtility::GetActivatableAbilities(const UAbilitySystemComponent* AbilitySystem)
{
	static const TArray<FGameplayAbilitySpec> None;
	check(None.IsEmpty())

	return FU_ENSURE(IsValid(AbilitySystem)) ? AbilitySystem->GetActivatableAbilities() : None;
}

inline const UGameplayAbility* UFuAbilityUtility::GetPrimaryAbilityInstance(UAbilitySystemComponent* AbilitySystem,
                                                                            const FGameplayAbilitySpecHandle AbilityHandle)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()))
	{
		return nullptr;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	return AbilitySpecification != nullptr ? AbilitySpecification->GetPrimaryInstance() : nullptr;
}

inline bool UFuAbilityUtility::IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem,
                                                              const FGameplayAbilitySpecHandle AbilityHandle)
{
	const auto* AbilityInstance{GetPrimaryAbilityInstance(AbilitySystem, AbilityHandle)};

	return IsValid(AbilityInstance) && AbilityInstance->IsActive();
}

inline bool UFuAbilityUtility::SwitchIsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem,
                                                                    const FGameplayAbilitySpecHandle AbilityHandle)
{
	return IsPrimaryAbilityInstanceActive(AbilitySystem, AbilityHandle);
}

inline bool UFuAbilityUtility::SwitchCanActivateAbilityByTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag)
{
	return CanActivateAbilityByTag(AbilitySystem, Tag);
}

inline bool UFuAbilityUtility::SwitchCanActivateAbilityByClass(UAbilitySystemComponent* AbilitySystem,
                                                               const TSubclassOf<UGameplayAbility> AbilityClass)
{
	return CanActivateAbilityByClass(AbilitySystem, AbilityClass);
}

inline bool UFuAbilityUtility::SwitchCanActivateAbilityByHandle(UAbilitySystemComponent* AbilitySystem,
                                                                const FGameplayAbilitySpecHandle AbilityHandle)
{
	return CanActivateAbilityByHandle(AbilitySystem, AbilityHandle);
}

inline void UFuAbilityUtility::CancelAbilityByHandle(UAbilitySystemComponent* AbilitySystem,
                                                     const FGameplayAbilitySpecHandle AbilityHandle)
{
	if (FU_ENSURE(IsValid(AbilitySystem)))
	{
		AbilitySystem->CancelAbilityHandle(AbilityHandle);
	}
}

inline void UFuAbilityUtility::CancelAbilitiesWithAnyTag(UAbilitySystemComponent* AbilitySystem,
                                                         const FGameplayTagContainer& Tags, UGameplayAbility* IgnoreAbility)
{
	if (FU_ENSURE(IsValid(AbilitySystem)) && FU_ENSURE(Tags.IsValid()))
	{
		AbilitySystem->CancelAbilities(&Tags, nullptr, IgnoreAbility);
	}
}

inline void UFuAbilityUtility::CancelAbilitiesWithoutAnyTag(UAbilitySystemComponent* AbilitySystem,
                                                            const FGameplayTagContainer& Tags, UGameplayAbility* IgnoreAbility)
{
	if (FU_ENSURE(IsValid(AbilitySystem)) && FU_ENSURE(Tags.IsValid()))
	{
		AbilitySystem->CancelAbilities(nullptr, &Tags, IgnoreAbility);
	}
}
