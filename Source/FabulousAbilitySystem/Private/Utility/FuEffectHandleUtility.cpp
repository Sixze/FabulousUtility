#include "Utility/FuEffectHandleUtility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEffectHandleUtility)

bool UFuEffectHandleUtility::IsActive(const FActiveGameplayEffectHandle EffectHandle)
{
	const auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};

	return IsValid(AbilitySystem) && AbilitySystem->GetActiveGameplayEffect(EffectHandle) != nullptr;
}

bool UFuEffectHandleUtility::IsActiveExpanded(const FActiveGameplayEffectHandle EffectHandle)
{
	return IsActive(EffectHandle);
}

void UFuEffectHandleUtility::RemoveActiveEffect(const FActiveGameplayEffectHandle EffectHandle, const int32 StacksToRemove)
{
	auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->RemoveActiveGameplayEffect(EffectHandle, StacksToRemove);
	}
}

void UFuEffectHandleUtility::RemoveActiveEffects(const TArray<FActiveGameplayEffectHandle>& EffectHandles, const int32 StacksToRemove)
{
	for (auto EffectHandle : EffectHandles)
	{
		auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
		if (IsValid(AbilitySystem))
		{
			AbilitySystem->RemoveActiveGameplayEffect(EffectHandle, StacksToRemove);
		}
	}
}

void UFuEffectHandleUtility::RecalculateEffectModifiers(const FActiveGameplayEffectHandle EffectHandle)
{
	// https://github.com/tranek/GASDocumentation#concepts-ge-definition

	const auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return;
	}

	auto& ActiveEffects{const_cast<FActiveGameplayEffectsContainer&>(AbilitySystem->GetActiveGameplayEffects())};

	const auto* ActiveEffect{ActiveEffects.GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect != nullptr)
	{
		ActiveEffects.SetActiveGameplayEffectLevel(EffectHandle, static_cast<int32>(ActiveEffect->Spec.GetLevel()));
	}
}

bool UFuEffectHandleUtility::TryGetEffectTimeRemainingAndDurationByHandle(const FActiveGameplayEffectHandle EffectHandle,
                                                                          float& TimeRemaining, float& Duration)
{
	TimeRemaining = -1.0f;
	Duration = -1.0f;

	const auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
	if (!IsValid(AbilitySystem))
	{
		return false;
	}

	const auto* ActiveEffect{AbilitySystem->GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect == nullptr)
	{
		return false;
	}

	Duration = ActiveEffect->GetDuration();

	TimeRemaining = Duration < 0.0f
		                ? FGameplayEffectConstants::INFINITE_DURATION
		                : UE_REAL_TO_FLOAT(Duration + ActiveEffect->StartWorldTime - AbilitySystem->GetWorld()->GetTimeSeconds());
	return true;
}

void UFuEffectHandleUtility::SetEffectDuration(const FActiveGameplayEffectHandle EffectHandle, float Duration)
{
	// https://github.com/tranek/GASDocumentation#concepts-ge-duration

	auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return;
	}

	auto& ActiveEffects{const_cast<FActiveGameplayEffectsContainer&>(AbilitySystem->GetActiveGameplayEffects())};
	auto* ActiveEffect{ActiveEffects.GetActiveGameplayEffect(EffectHandle)};

	if (ActiveEffect == nullptr ||
	    !FU_ENSURE_MESSAGE(ActiveEffect->GetDuration() > 0.0f, TEXT("Changing duration of instant or infinite effects is not allowed!")))
	{
		return;
	}

	if (!FU_ENSURE(Duration > 0.0f))
	{
		Duration = UE_KINDA_SMALL_NUMBER;
	}

	ActiveEffect->Spec.Duration = Duration;
	ActiveEffect->StartServerWorldTime = ActiveEffects.GetServerWorldTime();
	ActiveEffect->CachedStartServerWorldTime = ActiveEffect->StartServerWorldTime;
	ActiveEffect->StartWorldTime = ActiveEffects.GetWorldTime();

	ActiveEffects.CheckDuration(EffectHandle);

	ActiveEffect->EventSet.OnTimeChanged.Broadcast(ActiveEffect->Handle, ActiveEffect->StartWorldTime, ActiveEffect->Spec.Duration);

	AbilitySystem->OnGameplayEffectDurationChange(*ActiveEffect);

	ActiveEffects.MarkItemDirty(*ActiveEffect);
}

void UFuEffectHandleUtility::SetEffectTimeRemaining(const FActiveGameplayEffectHandle EffectHandle, float TimeRemaining)
{
	auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return;
	}

	auto& ActiveEffects{const_cast<FActiveGameplayEffectsContainer&>(AbilitySystem->GetActiveGameplayEffects())};
	auto* ActiveEffect{ActiveEffects.GetActiveGameplayEffect(EffectHandle)};

	if (ActiveEffect == nullptr ||
	    !FU_ENSURE_MESSAGE(ActiveEffect->GetDuration() > 0.0f, TEXT("Changing duration of instant or infinite effects is not allowed!")))
	{
		return;
	}

	if (!FU_ENSURE(TimeRemaining > 0.0f))
	{
		TimeRemaining = UE_KINDA_SMALL_NUMBER;
	}

	const auto Time{ActiveEffect->GetDuration() - TimeRemaining};
	if (Time >= 0.0f)
	{
		ActiveEffect->StartServerWorldTime = ActiveEffects.GetServerWorldTime() - Time;
		ActiveEffect->CachedStartServerWorldTime = ActiveEffect->StartServerWorldTime;
		ActiveEffect->StartWorldTime = ActiveEffects.GetWorldTime() - Time;
	}
	else
	{
		// If the time remaining is greater than the duration, then adjust the duration to match the time remaining.

		ActiveEffect->Spec.Duration = TimeRemaining;
		ActiveEffect->StartServerWorldTime = ActiveEffects.GetServerWorldTime() - TimeRemaining;
		ActiveEffect->CachedStartServerWorldTime = ActiveEffect->StartServerWorldTime;
		ActiveEffect->StartWorldTime = ActiveEffects.GetWorldTime() - TimeRemaining;
	}

	ActiveEffects.CheckDuration(EffectHandle);

	ActiveEffect->EventSet.OnTimeChanged.Broadcast(ActiveEffect->Handle, ActiveEffect->StartWorldTime, ActiveEffect->GetDuration());

	AbilitySystem->OnGameplayEffectDurationChange(*ActiveEffect);

	ActiveEffects.MarkItemDirty(*ActiveEffect);
}

void UFuEffectHandleUtility::IncreaseEffectTimeRemaining(const FActiveGameplayEffectHandle EffectHandle,
                                                         const float AdditionalTimeRemaining)
{
	const auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return;
	}

	const auto& ActiveEffects{AbilitySystem->GetActiveGameplayEffects()};
	auto* ActiveEffect{ActiveEffects.GetActiveGameplayEffect(EffectHandle)};

	if (ActiveEffect != nullptr)
	{
		SetEffectTimeRemaining(EffectHandle,
		                       ActiveEffect->GetTimeRemaining(ActiveEffects.GetWorldTime()) + AdditionalTimeRemaining);
	}
}
