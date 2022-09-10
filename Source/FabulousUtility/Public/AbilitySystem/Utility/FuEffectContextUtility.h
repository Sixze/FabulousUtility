#pragma once

#include "FuMacros.h"
#include "AbilitySystem/FuGameplayEffectContext.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectContextUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuEffectContextUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Context Utility", Meta = (AutoCreateRefTerm = "EffectContextHandle"))
	static FGameplayEffectContextHandle DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Context Utility", Meta = (AutoCreateRefTerm = "EffectContextHandle"))
	static FGameplayAbilityTargetDataHandle GetTargetDataHandleFromEffectContext(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Context Utility",
		Meta = (AutoCreateRefTerm = "EffectContextHandle, TargetDataHandle"))
	static void AddTargetDataToEffectContext(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                         const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Context Utility",
		Meta = (AutoCreateRefTerm = "EffectContextHandle, TargetDataHandle"))
	static void SetTargetDataToEffectContext(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                                         const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Context Utility",
		Meta = (AutoCreateRefTerm = "EffectContextHandle, TargetDataHandle"))
	static void ClearTargetDataInEffectContext(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);
};

inline FGameplayEffectContextHandle UFuEffectContextUtility::DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContextHandle)
{
	return EffectContextHandle.Duplicate();
}

inline FGameplayAbilityTargetDataHandle UFuEffectContextUtility::GetTargetDataHandleFromEffectContext(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	return FU_ENSURE(EffectContextHandle.IsValid()) &&
	       FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct()))
		       ? static_cast<const FFuGameplayEffectContext*>(EffectContextHandle.Get())->GetTargetDataHandle()
		       : FGameplayAbilityTargetDataHandle{};
}

inline void UFuEffectContextUtility::AddTargetDataToEffectContext(FGameplayEffectContextHandle& EffectContextHandle,
                                                                  const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->AddTargetDataHandle(TargetDataHandle);
	}
}

inline void UFuEffectContextUtility::SetTargetDataToEffectContext(FGameplayEffectContextHandle& EffectContextHandle,
                                                                  const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->SetTargetDataHandle(TargetDataHandle);
	}
}

inline void UFuEffectContextUtility::ClearTargetDataInEffectContext(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->ClearTargetDataHandle();
	}
}
