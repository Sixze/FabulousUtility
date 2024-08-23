#include "EffectComponents/FuAnyTagRemovalRequirementEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Misc/EnumerateRange.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnyTagRemovalRequirementEffectComponent)

UFuAnyTagRemovalRequirementEffectComponent::UFuAnyTagRemovalRequirementEffectComponent()
{
#if WITH_EDITORONLY_DATA
	EditorFriendlyName = FString{TEXTVIEW("Removal Requirement (Any Tag)")};
#endif
}

void UFuAnyTagRemovalRequirementEffectComponent::PostInitProperties()
{
	Super::PostInitProperties();

	RefreshRemovalRequirementTags();
}

#if WITH_EDITOR
void UFuAnyTagRemovalRequirementEffectComponent::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(ThisClass, RemovalRequirementTags))
	{
		RefreshRemovalRequirementTags();
	}

	Super::PostEditChangeProperty(ChangedEvent);
}
#endif

bool UFuAnyTagRemovalRequirementEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveEffects,
                                                                        const FGameplayEffectSpec& EffectSpecification) const
{
	return !ActiveEffects.Owner->HasAnyMatchingGameplayTags(RemovalRequirementTags.CombinedTags);
}

bool UFuAnyTagRemovalRequirementEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveEffects,
                                                                             FActiveGameplayEffect& ActiveEffect) const
{
	if (RemovalRequirementTags.CombinedTags.IsEmpty())
	{
		return true;
	}

	auto* AbilitySystem{ActiveEffects.Owner};

	TArray<FDelegateHandle> TagChangedDelegateHandles;
	TagChangedDelegateHandles.Reserve(RemovalRequirementTags.CombinedTags.Num());

	for (const auto& Tag : RemovalRequirementTags.CombinedTags)
	{
		const auto DelegateHandle{
			AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
			             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged, ActiveEffect.Handle)
		};

		TagChangedDelegateHandles.Emplace(DelegateHandle);
	}

	ActiveEffect.EventSet.OnEffectRemoved.AddUObject(this, &ThisClass::Effect_OnRemoved,
	                                                 // ReSharper disable once CppBoundToDelegateMethodIsNotMarkedAsUFunction
	                                                 AbilitySystem, MoveTemp(TagChangedDelegateHandles));
	return true;
}

void UFuAnyTagRemovalRequirementEffectComponent::OnGameplayEffectChanged()
{
	RefreshRemovalRequirementTags();
}

void UFuAnyTagRemovalRequirementEffectComponent::RefreshRemovalRequirementTags()
{
	const auto* ParentComponent{FindParentComponent(*this)};

	RemovalRequirementTags.UpdateInheritedTagProperties(IsValid(ParentComponent) ? &ParentComponent->RemovalRequirementTags : nullptr);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UFuAnyTagRemovalRequirementEffectComponent::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 TagCount,
                                                                            const FActiveGameplayEffectHandle EffectHandle) const
{
	auto* AbilitySystem{EffectHandle.GetOwningAbilitySystemComponent()};

	if (IsValid(AbilitySystem) && TagCount > 0)
	{
		AbilitySystem->RemoveActiveGameplayEffect(EffectHandle);
	}
}

void UFuAnyTagRemovalRequirementEffectComponent::Effect_OnRemoved(const FGameplayEffectRemovalInfo& RemovalInfo,
                                                                  UAbilitySystemComponent* AbilitySystem,
                                                                  TArray<FDelegateHandle> TagChangedDelegateHandles) const
{
	for (const auto DelegateHandle : EnumerateRange(TagChangedDelegateHandles))
	{
		FU_ENSURE(AbilitySystem->UnregisterGameplayTagEvent(*DelegateHandle,
			RemovalRequirementTags.CombinedTags.GetGameplayTagArray()[DelegateHandle.GetIndex()], EGameplayTagEventType::NewOrRemoved));
	}
}
