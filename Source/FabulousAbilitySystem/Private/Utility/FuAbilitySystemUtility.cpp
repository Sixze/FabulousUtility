#include "Utility/FuAbilitySystemUtility.h"

#include "AbilitySystemInterface.h"
#include "FuGameplayTagUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilitySystemUtility)

UAbilitySystemComponent* UFuAbilitySystemUtility::GetAbilitySystem(const UObject* Object)
{
	const auto* Accessor{Cast<IAbilitySystemInterface>(Object)};
	if (Accessor != nullptr)
	{
		auto* AbilitySystem{Accessor->GetAbilitySystemComponent()};
		if (IsValid(AbilitySystem))
		{
			return AbilitySystem;
		}
	}

	const auto* Actor{Cast<AActor>(Object)};
	if (IsValid(Actor))
	{
		return Actor->FindComponentByClass<UAbilitySystemComponent>();
	}

	return nullptr;
}

bool UFuAbilitySystemUtility::TryGetAbilitySystem(const UObject* Object, UAbilitySystemComponent*& AbilitySystem)
{
	const auto* Accessor{Cast<IAbilitySystemInterface>(Object)};
	if (Accessor != nullptr)
	{
		AbilitySystem = Accessor->GetAbilitySystemComponent();
		if (IsValid(AbilitySystem))
		{
			return true;
		}
	}

	const auto* Actor{Cast<AActor>(Object)};
	if (IsValid(Actor))
	{
		AbilitySystem = Actor->FindComponentByClass<UAbilitySystemComponent>();
		return IsValid(AbilitySystem);
	}

	AbilitySystem = nullptr;
	return false;
}

FGameplayTag UFuAbilitySystemUtility::FindFirstOwnedDescendantTag(const UAbilitySystemComponent* AbilitySystem,
                                                                  const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return FGameplayTag::EmptyTag;
	}

	const auto& Tags{AbilitySystem->GetOwnedGameplayTags()};

	return UFuGameplayTagUtility::FindFirstDescendantTag(Tags, Tag);
}

FGameplayAbilitySpecHandle UFuAbilitySystemUtility::GiveAbilityWithDynamicTags(UAbilitySystemComponent* AbilitySystem,
                                                                               const TSubclassOf<UGameplayAbility> AbilityClass,
                                                                               const int32 Level, const FGameplayTagContainer& Tags,
                                                                               UObject* SourceObject)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return {};
	}

	auto AbilitySpecification{AbilitySystem->BuildAbilitySpecFromClass(AbilityClass, Level)};
	if (!IsValid(AbilitySpecification.Ability))
	{
		return {};
	}

	for (const auto& Tag : Tags)
	{
		if (Tag.IsValid())
		{
			AbilitySpecification.GetDynamicSpecSourceTags().AddTag(Tag);
		}
	}

	AbilitySpecification.SourceObject = SourceObject;

	return AbilitySystem->GiveAbility(AbilitySpecification);
}

FActiveGameplayEffectHandle UFuAbilitySystemUtility::ApplyEffectWithSetByCallerMagnitudes(
	UAbilitySystemComponent* AbilitySystem, const TSubclassOf<UGameplayEffect> EffectClass,
	const TMap<FGameplayTag, float>& SetByCallerMagnitudes)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass)))
	{
		return {};
	}

	FGameplayEffectSpec EffectSpecification{EffectClass.GetDefaultObject(), AbilitySystem->MakeEffectContext(), 1.0f};
	EffectSpecification.SetByCallerTagMagnitudes = SetByCallerMagnitudes;

	return AbilitySystem->ApplyGameplayEffectSpecToSelf(EffectSpecification);
}

FActiveGameplayEffectHandle UFuAbilitySystemUtility::ApplyEffectWithSetByCallerMagnitudes(
	UAbilitySystemComponent* AbilitySystem, const TSubclassOf<UGameplayEffect> EffectClass,
	TMap<FGameplayTag, float>&& SetByCallerMagnitudes)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass)))
	{
		return {};
	}

	FGameplayEffectSpec EffectSpecification{EffectClass.GetDefaultObject(), AbilitySystem->MakeEffectContext(), 1.0f};
	EffectSpecification.SetByCallerTagMagnitudes = MoveTemp(SetByCallerMagnitudes);

	return AbilitySystem->ApplyGameplayEffectSpecToSelf(EffectSpecification);
}

bool UFuAbilitySystemUtility::AddLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, const bool bReplicate)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	AbilitySystem->AddLooseGameplayTag(Tag, 1, bReplicate
		                                           ? EGameplayTagReplicationState::CountToOwner
		                                           : EGameplayTagReplicationState::None);

	return true;
}

bool UFuAbilitySystemUtility::RemoveLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	AbilitySystem->RemoveLooseGameplayTag(Tag);

	return true;
}

bool UFuAbilitySystemUtility::AddLooseTags(UAbilitySystemComponent* AbilitySystem,
                                           const FGameplayTagContainer& Tags, const bool bReplicate)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	AbilitySystem->AddLooseGameplayTags(Tags, 1, bReplicate
		                                             ? EGameplayTagReplicationState::CountToOwner
		                                             : EGameplayTagReplicationState::None);

	return true;
}

bool UFuAbilitySystemUtility::RemoveLooseTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	AbilitySystem->RemoveLooseGameplayTags(Tags);

	return true;
}
