#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "FuGameplayEffectContext.generated.h"

USTRUCT()
struct FABULOUSABILITYSYSTEM_API FFuGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FGameplayAbilityTargetDataHandle TargetDataHandle;

public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToString() const override;

	virtual FGameplayEffectContext* Duplicate() const override;

	virtual bool NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess) override;

public:
	FGameplayAbilityTargetDataHandle GetTargetDataHandle() const;

	void AppendTargetDataHandle(const FGameplayAbilityTargetDataHandle& NewTargetDataHandle);

	void SetTargetDataHandle(const FGameplayAbilityTargetDataHandle& NewTargetDataHandle);

	void ClearTargetDataHandle();
};

template <>
struct TStructOpsTypeTraits<FFuGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FFuGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

inline FGameplayAbilityTargetDataHandle FFuGameplayEffectContext::GetTargetDataHandle() const
{
	return TargetDataHandle;
}

inline void FFuGameplayEffectContext::AppendTargetDataHandle(const FGameplayAbilityTargetDataHandle& NewTargetDataHandle)
{
	TargetDataHandle.Append(NewTargetDataHandle);
}

inline void FFuGameplayEffectContext::SetTargetDataHandle(const FGameplayAbilityTargetDataHandle& NewTargetDataHandle)
{
	TargetDataHandle = NewTargetDataHandle;
}

inline void FFuGameplayEffectContext::ClearTargetDataHandle()
{
	TargetDataHandle.Clear();
}
