#pragma once

#include "FuGameplayEffectContext.h"
#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectContextUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectContextUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Context Utility", Meta = (ReturnDisplayName = "Effect Context Handle"))
	static FGameplayEffectContextHandle DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Context Utility", Meta = (ReturnDisplayName = "Target Data Handle"))
	static FGameplayAbilityTargetDataHandle GetTargetDataHandle(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Context Utility")
	static void AppendTargetData(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                             const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Context Utility")
	static void SetTargetData(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	                          const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Context Utility")
	static void ClearTargetData(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle);
};

inline FGameplayEffectContextHandle UFuEffectContextUtility::DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContextHandle)
{
	return EffectContextHandle.Duplicate();
}

inline FGameplayAbilityTargetDataHandle UFuEffectContextUtility::GetTargetDataHandle(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	return FU_ENSURE(EffectContextHandle.IsValid()) &&
	       FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct()))
		       ? static_cast<const FFuGameplayEffectContext*>(EffectContextHandle.Get())->GetTargetDataHandle()
		       : FGameplayAbilityTargetDataHandle{};
}

inline void UFuEffectContextUtility::AppendTargetData(FGameplayEffectContextHandle& EffectContextHandle,
                                                      const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->AppendTargetDataHandle(TargetDataHandle);
	}
}

inline void UFuEffectContextUtility::SetTargetData(FGameplayEffectContextHandle& EffectContextHandle,
                                                   const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->SetTargetDataHandle(TargetDataHandle);
	}
}

inline void UFuEffectContextUtility::ClearTargetData(FGameplayEffectContextHandle& EffectContextHandle)
{
	if (FU_ENSURE(EffectContextHandle.IsValid()) &&
	    FU_ENSURE(EffectContextHandle.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContextHandle.Get())->ClearTargetDataHandle();
	}
}
