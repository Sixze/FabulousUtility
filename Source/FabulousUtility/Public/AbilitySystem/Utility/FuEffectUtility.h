#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "AbilitySystem/FuGameplayEffectContext.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuEffectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Effect

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility")
	static const FGameplayTagContainer& GetEffectOwnedTags(TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility")
	static bool IsEffectActive(FActiveGameplayEffectHandle EffectHandle);

	// Effect Context

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectContextHandle"))
	static FGameplayEffectContextHandle DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectContextHandle"))
	static FGameplayAbilityTargetDataHandle GetTargetDataHandleFromEffectContext(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectContextHandle, TargetDataHandle"))
	static void AddTargetDataToEffectContext(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                         const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectContextHandle, TargetDataHandle"))
	static void SetTargetDataToEffectContext(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                         const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectContextHandle, TargetDataHandle"))
	static void ClearTargetDataInEffectContext(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);
};

inline const FGameplayTagContainer& UFuEffectUtility::GetEffectOwnedTags(const TSubclassOf<UGameplayEffect> EffectClass)
{
	if (FU_ENSURE(IsValid(EffectClass)))
	{
		return EffectClass.GetDefaultObject()->InheritableOwnedTagsContainer.CombinedTags;
	}

	static const FGameplayTagContainer None;
	check(None.IsEmpty())

	return None;
}

inline bool UFuEffectUtility::IsEffectActive(const FActiveGameplayEffectHandle EffectHandle)
{
	const auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};

	return IsValid(AbilitySystem) && AbilitySystem->GetActiveGameplayEffect(EffectHandle) != nullptr;
}

inline FGameplayEffectContextHandle UFuEffectUtility::DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContextHandle)
{
	return EffectContextHandle.Duplicate();
}

inline FGameplayAbilityTargetDataHandle UFuEffectUtility::GetTargetDataHandleFromEffectContext(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	return FU_ENSURE(EffectContextHandle.IsValid()) &&
	       FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct()))
		       ? static_cast<const FFuGameplayEffectContext*>(EffectContextHandle.Get())->GetTargetDataHandle()
		       : FGameplayAbilityTargetDataHandle{};
}

inline void UFuEffectUtility::AddTargetDataToEffectContext(FGameplayEffectContextHandle& EffectContextHandle,
                                                           const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->AddTargetDataHandle(TargetDataHandle);
	}
}

inline void UFuEffectUtility::SetTargetDataToEffectContext(FGameplayEffectContextHandle& EffectContextHandle,
                                                           const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->SetTargetDataHandle(TargetDataHandle);
	}
}

inline void UFuEffectUtility::ClearTargetDataInEffectContext(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->ClearTargetDataHandle();
	}
}
