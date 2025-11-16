#include "FuGameplayEffectContext.h"

#include "FuMacros.h"
// #include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
// #include "Serialization/GameplayEffectContextNetSerializer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayEffectContext)

// TODO Create a custom NetSerializer to properly replicate this structure via Iris, or wait until Epic provides a way to avoid it.

// namespace UE::Net
// {
// 	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(FuGameplayEffectContext, FGameplayEffectContextNetSerializer);
// }

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

	if (HitResult.IsValid())
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
