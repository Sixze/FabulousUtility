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
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Context Utility", Meta = (ReturnDisplayName = "Effect Context"))
	static FGameplayEffectContextHandle DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Context Utility", Meta = (ReturnDisplayName = "Target Data Handle"))
	static FGameplayAbilityTargetDataHandle GetTargetDataHandle(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Context Utility", Meta = (ReturnDisplayName = "Effect Context"))
	static FGameplayEffectContextHandle& AppendTargetData(UPARAM(ref) FGameplayEffectContextHandle& EffectContext,
	                                                      const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Context Utility", Meta = (ReturnDisplayName = "Effect Context"))
	static FGameplayEffectContextHandle& SetTargetData(UPARAM(ref) FGameplayEffectContextHandle& EffectContext,
	                                                   const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Context Utility", Meta = (ReturnDisplayName = "Effect Context"))
	static FGameplayEffectContextHandle& ClearTargetData(UPARAM(ref) FGameplayEffectContextHandle& EffectContext);
};

inline FGameplayEffectContextHandle UFuEffectContextUtility::DuplicateEffectContext(const FGameplayEffectContextHandle& EffectContext)
{
	return EffectContext.Duplicate();
}

inline FGameplayAbilityTargetDataHandle UFuEffectContextUtility::GetTargetDataHandle(const FGameplayEffectContextHandle& EffectContext)
{
	if (!FU_ENSURE(EffectContext.IsValid()) ||
	    !FU_ENSURE(EffectContext.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		return {};
	}

	return static_cast<const FFuGameplayEffectContext*>(EffectContext.Get())->GetTargetDataHandle();
}

inline FGameplayEffectContextHandle& UFuEffectContextUtility::AppendTargetData(FGameplayEffectContextHandle& EffectContext,
                                                                               const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContext.IsValid()) &&
	    FU_ENSURE(EffectContext.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContext.Get())->AppendTargetDataHandle(TargetDataHandle);
	}

	return EffectContext;
}

inline FGameplayEffectContextHandle& UFuEffectContextUtility::SetTargetData(FGameplayEffectContextHandle& EffectContext,
                                                                            const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (FU_ENSURE(EffectContext.IsValid()) &&
	    FU_ENSURE(EffectContext.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContext.Get())->SetTargetDataHandle(TargetDataHandle);
	}

	return EffectContext;
}

inline FGameplayEffectContextHandle& UFuEffectContextUtility::ClearTargetData(FGameplayEffectContextHandle& EffectContext)
{
	if (FU_ENSURE(EffectContext.IsValid()) &&
	    FU_ENSURE(EffectContext.Get()->GetScriptStruct()->IsChildOf(FFuGameplayEffectContext::StaticStruct())))
	{
		static_cast<FFuGameplayEffectContext*>(EffectContext.Get())->ClearTargetDataHandle();
	}

	return EffectContext;
}
