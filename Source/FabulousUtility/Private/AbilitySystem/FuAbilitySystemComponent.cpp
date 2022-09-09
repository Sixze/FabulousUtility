#include "AbilitySystem/FuAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "FuMacros.h"
#include "AbilitySystem/FuGameplayAbility.h"
#include "AbilitySystem/FuGameplayEffect.h"

bool UFuAbilitySystemComponent::TryGetFuAbilitySystem(const UObject* Object, ThisClass*& AbilitySystem, const bool bAllowFindComponent)
{
	const auto* Accessor{Cast<IAbilitySystemInterface>(Object)};
	if (Accessor != nullptr)
	{
		AbilitySystem = Cast<ThisClass>(Accessor->GetAbilitySystemComponent());
		if (IsValid(AbilitySystem))
		{
			return true;
		}
	}

	const auto Actor{Cast<AActor>(Object)};
	if (bAllowFindComponent && IsValid(Actor))
	{
		AbilitySystem = Actor->FindComponentByClass<UFuAbilitySystemComponent>();
		return IsValid(AbilitySystem);
	}

	AbilitySystem = nullptr;
	return false;
}

void UFuAbilitySystemComponent::OnRegister()
{
	Super::OnRegister();

	// Subscribe to the event here after calling Super::OnRegister() so that UFuAbilitySystemComponent::OnAnyTagChanged()
	// is called before FActiveGameplayEffectsContainer::OnOwnerTagChange().

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnGameplayEffectApplied);

	RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::OnAnyTagChanged);
}

FActiveGameplayEffectHandle UFuAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& EffectSpecification,
                                                                                     const FPredictionKey PredictionKey)
{
	// The code block below should be called after the call to FActiveGameplayEffectsContainer::HasApplicationImmunityToSpec()
	// in the UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf() function, but currently it is not possible.

	const auto* Effect{Cast<UFuGameplayEffect>(EffectSpecification.Def)};

	if (IsValid(Effect) && Effect->GetRemovalRequirementAnyTag().HasAny(GameplayTagCountContainer.GetExplicitGameplayTags()))
	{
		return {};
	}

	return Super::ApplyGameplayEffectSpecToSelf(EffectSpecification, PredictionKey);
}

void UFuAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(AbilityHandle, Ability);

	OnAbilityActivated.Broadcast(AbilityHandle, Ability);
}

void UFuAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability,
                                                    const FGameplayTagContainer& FailureTags)
{
	Super::NotifyAbilityFailed(AbilityHandle, Ability, FailureTags);

	OnAbilityFailed.Broadcast(AbilityHandle, Ability, FailureTags);
}

bool UFuAbilitySystemComponent::ShouldDoServerAbilityRPCBatch() const
{
	return true;
}

void UFuAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpecification)
{
	Super::OnGiveAbility(AbilitySpecification);

	OnAbilityGiven.Broadcast(this, AbilitySpecification);
}

void UFuAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpecification)
{
	Super::OnRemoveAbility(AbilitySpecification);

	OnAbilityRemoved.Broadcast(this, AbilitySpecification);
}

bool UFuAbilitySystemComponent::AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const
{
	return Super::AreAbilityTagsBlocked(Tags) ||
	       // ReSharper disable once CppRedundantParentheses
	       (BlockedAbilityWithoutTags.GetExplicitGameplayTags().IsValid() &&
	        !Tags.HasAny(BlockedAbilityWithoutTags.GetExplicitGameplayTags()));
}

void UFuAbilitySystemComponent::AbilityLocalInputPressed(const int32 InputId)
{
	// Based on UAbilitySystemComponent::AbilityLocalInputPressed().

	if (IsGenericConfirmInputBound(InputId))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputId))
	{
		LocalInputCancel();
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	for (auto& AbilitySpecification : GetActivatableAbilities())
	{
		if (AbilitySpecification.InputID != InputId || !IsValid(AbilitySpecification.Ability))
		{
			continue;
		}

		AbilitySpecification.InputPressed = true;

		if (AbilitySpecification.IsActive())
		{
			if (AbilitySpecification.Ability->bReplicateInputDirectly && !IsOwnerActorAuthoritative())
			{
				ServerSetInputPressed(AbilitySpecification.Handle);
			}

			AbilitySpecInputPressed(AbilitySpecification);

			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpecification.Handle,
			                      AbilitySpecification.ActivationInfo.GetActivationPredictionKey());
			continue;
		}

		const auto* FuAbility{Cast<UFuGameplayAbility>(AbilitySpecification.Ability)};

		if (!IsValid(FuAbility) || FuAbility->IsActivationByInputAllowed())
		{
			TryActivateAbility(AbilitySpecification.Handle);
		}
	}
}

bool UFuAbilitySystemComponent::HasAnyActiveEffectsByQuery(const FGameplayEffectQuery& EffectQuery) const
{
	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			return true;
		}
	}

	return false;
}

void UFuAbilitySystemComponent::GetActiveEffectsByQuery(const FGameplayEffectQuery& EffectQuery,
                                                        TArray<FActiveGameplayEffect>& ActiveEffects) const
{
	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			ActiveEffects.Add(ActiveEffect);
		}
	}
}

bool UFuAbilitySystemComponent::HasAnyActiveEffectsWithTag(const FGameplayTag& EffectTag, const bool bIgnoreInhibitedEffects) const
{
	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			return true;
		}
	}

	return false;
}

bool UFuAbilitySystemComponent::HasAnyActiveEffectsWithAnyTags(const FGameplayTagContainer& EffectTags,
                                                               const bool bIgnoreInhibitedEffects) const
{
	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags))
		{
			return true;
		}
	}

	return false;
}

int32 UFuAbilitySystemComponent::GetEffectsCountWithTag(const FGameplayTag& EffectTag, const bool bIgnoreInhibitedEffects) const
{
	auto Count{0};

	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			Count += ActiveEffect.Spec.StackCount;
		}
	}

	return Count;
}

int32 UFuAbilitySystemComponent::GetEffectsCountWithAnyTags(const FGameplayTagContainer& EffectTags,
                                                            const bool bIgnoreInhibitedEffects) const
{
	auto Count{0};

	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags))
		{
			Count += ActiveEffect.Spec.StackCount;
		}
	}

	return Count;
}

int32 UFuAbilitySystemComponent::GetEffectStacksCountByClass(const TSubclassOf<UGameplayEffect> EffectClass) const
{
	if (!FU_ENSURE(IsValid(EffectClass)) || !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		return 0;
	}

	auto MaxCount{0};

	for (const auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.Spec.Def->GetClass() == EffectClass)
		{
			MaxCount = FMath::Max(MaxCount, ActiveEffect.Spec.StackCount);
		}
	}

	return MaxCount;
}

void UFuAbilitySystemComponent::RecalculateEffectModifiers(const FActiveGameplayEffectHandle EffectHandle)
{
	// https://github.com/tranek/GASDocumentation#concepts-ge-definition

	const auto* ActiveEffect{ActiveGameplayEffects.GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect != nullptr)
	{
		ActiveGameplayEffects.SetActiveGameplayEffectLevel(EffectHandle, ActiveEffect->Spec.GetLevel());
	}
}

void UFuAbilitySystemComponent::SetEffectDuration(const FActiveGameplayEffectHandle EffectHandle, float Duration)
{
	// https://github.com/tranek/GASDocumentation#concepts-ge-duration

	auto* ActiveEffect{ActiveGameplayEffects.GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect == nullptr)
	{
		return;
	}

	Duration = FMath::Max(0.01f, Duration);
	const auto Time{ActiveGameplayEffects.GetWorldTime() - ActiveEffect->StartWorldTime};

	ActiveEffect->Spec.Duration = Duration;

	if (Time > Duration)
	{
		ActiveEffect->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime() - Duration;
		ActiveEffect->CachedStartServerWorldTime = ActiveEffect->StartServerWorldTime;
		ActiveEffect->StartWorldTime = ActiveGameplayEffects.GetWorldTime() - Duration;
	}

	ActiveGameplayEffects.CheckDuration(EffectHandle);

	ActiveEffect->EventSet.OnTimeChanged.Broadcast(ActiveEffect->Handle, ActiveEffect->StartWorldTime, ActiveEffect->Spec.Duration);

	OnGameplayEffectDurationChange(*ActiveEffect);

	ActiveGameplayEffects.MarkItemDirty(*ActiveEffect);
}

void UFuAbilitySystemComponent::SetEffectTimeRemaining(const FActiveGameplayEffectHandle EffectHandle, float TimeRemaining)
{
	auto* ActiveEffect{ActiveGameplayEffects.GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect == nullptr)
	{
		return;
	}

	TimeRemaining = FMath::Max(0.0f, TimeRemaining);
	const auto Time{ActiveEffect->GetDuration() - TimeRemaining};

	if (Time >= 0.0f)
	{
		ActiveEffect->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime() - Time;
		ActiveEffect->CachedStartServerWorldTime = ActiveEffect->StartServerWorldTime;
		ActiveEffect->StartWorldTime = ActiveGameplayEffects.GetWorldTime() - Time;
	}
	else
	{
		// If the time remaining is greater than the duration, then adjust the duration to match the time remaining.

		ActiveEffect->Spec.Duration -= Time;

		ActiveEffect->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime() - TimeRemaining;
		ActiveEffect->CachedStartServerWorldTime = ActiveEffect->StartServerWorldTime;
		ActiveEffect->StartWorldTime = ActiveGameplayEffects.GetWorldTime() - TimeRemaining;
	}

	ActiveGameplayEffects.CheckDuration(EffectHandle);

	ActiveEffect->EventSet.OnTimeChanged.Broadcast(ActiveEffect->Handle, ActiveEffect->StartWorldTime, ActiveEffect->GetDuration());

	OnGameplayEffectDurationChange(*ActiveEffect);

	ActiveGameplayEffects.MarkItemDirty(*ActiveEffect);
}

void UFuAbilitySystemComponent::IncreaseEffectTimeRemaining(const FActiveGameplayEffectHandle EffectHandle,
                                                            const float AdditionalTimeRemaining)
{
	const auto* ActiveEffect{ActiveGameplayEffects.GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect != nullptr)
	{
		SetEffectTimeRemaining(EffectHandle,
		                       ActiveEffect->GetTimeRemaining(ActiveGameplayEffects.GetWorldTime()) + AdditionalTimeRemaining);
	}
}

const FActiveGameplayEffect* UFuAbilitySystemComponent::GetActiveEffectTimeRemainingAndDurationByTag(
	const FGameplayTag& EffectTag, float& TimeRemaining, float& Duration) const
{
	const FActiveGameplayEffect* Result{nullptr};

	TimeRemaining = -1.0f;
	Duration = -1.0f;

	const auto Time{ActiveGameplayEffects.GetWorldTime()};

	for (auto& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			const auto OtherDuration{ActiveEffect.GetDuration()};
			const auto OtherTimeRemaining{OtherDuration + ActiveEffect.StartWorldTime - Time};

			if (OtherTimeRemaining > TimeRemaining)
			{
				Result = &ActiveEffect;

				TimeRemaining = OtherTimeRemaining;
				Duration = OtherDuration;
			}
		}
	}

	return Result;
}

void UFuAbilitySystemComponent::BlockAbilitiesWithoutTags(const FGameplayTagContainer& Tags)
{
	BlockedAbilityWithoutTags.UpdateTagCount(Tags, 1);
}

void UFuAbilitySystemComponent::UnBlockAbilitiesWithoutTags(const FGameplayTagContainer& Tags)
{
	BlockedAbilityWithoutTags.UpdateTagCount(Tags, -1);
}

void UFuAbilitySystemComponent::OnGameplayEffectApplied(UAbilitySystemComponent* InstigatorAbilitySystem,
                                                        const FGameplayEffectSpec& EffectSpecification,
                                                        const FActiveGameplayEffectHandle EffectHandle)
{
	// The code block below should be called right after the call to
	// FActiveGameplayEffectsContainer::AttemptRemoveActiveEffectsOnEffectApplication() in
	// the UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf() function, but currently it is not possible.

	if (IsOwnerActorAuthoritative())
	{
		const auto* Tags{EffectSpecification.CapturedTargetTags.GetAggregatedTags()};
		FGameplayEffectQuery EffectQuery;

		EffectQuery.CustomMatchDelegate.BindLambda([Tags](const FActiveGameplayEffect& ActiveEffect)
		{
			const auto* Effect{Cast<UFuGameplayEffect>(ActiveEffect.Spec.Def)};

			return IsValid(Effect) && Effect->GetRemovalRequirementAnyTag().HasAny(*Tags);
		});

		ActiveGameplayEffects.RemoveActiveEffects(EffectQuery, -1);
	}
}

void UFuAbilitySystemComponent::OnAnyTagChanged(const FGameplayTag Tag, const int32 NewCount)
{
	// Unfortunately, there is currently no way to optimize this the way it
	// is done inside FActiveGameplayEffectsContainer::OnOwnerTagChange().

	if (NewCount > 0)
	{
		const auto& Tags{GameplayTagCountContainer.GetExplicitGameplayTags()};
		FGameplayEffectQuery EffectQuery;

		EffectQuery.CustomMatchDelegate.BindLambda([&Tags](const FActiveGameplayEffect& ActiveEffect)
		{
			const auto* Effect{Cast<UFuGameplayEffect>(ActiveEffect.Spec.Def)};

			return IsValid(Effect) && Effect->GetRemovalRequirementAnyTag().HasAny(Tags);
		});

		ActiveGameplayEffects.RemoveActiveEffects(EffectQuery, -1);
	}
}

void UFuAbilitySystemComponent::GetEffectTimeRemainingAndDurationByTag(const FGameplayTag& EffectTag,
                                                                       float& TimeRemaining, float& Duration) const
{
	GetActiveEffectTimeRemainingAndDurationByTag(EffectTag, TimeRemaining, Duration);
}
