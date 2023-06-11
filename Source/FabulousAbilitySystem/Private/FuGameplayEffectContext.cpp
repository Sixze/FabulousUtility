#include "FuGameplayEffectContext.h"

#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayEffectContext)

UScriptStruct* FFuGameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

FString FFuGameplayEffectContext::ToString() const
{
	return FString{FU_GET_TYPE_STRING(FFuGameplayEffectContext)};
}

FGameplayEffectContext* FFuGameplayEffectContext::Duplicate() const
{
	auto* NewEffectContext{new FFuGameplayEffectContext{*this}};

	if (HitResult != nullptr)
	{
		NewEffectContext->AddHitResult(*HitResult, true);
	}

	return NewEffectContext;
}

bool FFuGameplayEffectContext::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	bSuccess = true;
	auto bSuccessLocal{true};

	Super::NetSerialize(Archive, Map, bSuccessLocal);
	bSuccess &= bSuccessLocal;

	TargetDataHandle.NetSerialize(Archive, Map, bSuccessLocal);
	bSuccess &= bSuccessLocal;

	return bSuccess;
}
