#include "AbilitySystem/Utility/FuAbilitySystemUtility.h"

#include "AbilitySystemInterface.h"
#include "FuGameplayTagUtility.h"

bool UFuAbilitySystemUtility::TryGetAbilitySystem(const UObject* Object, UAbilitySystemComponent*& AbilitySystem,
                                                  const bool bAllowFindComponent)
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

	const auto Actor{Cast<AActor>(Object)};
	if (bAllowFindComponent && IsValid(Actor))
	{
		AbilitySystem = Actor->FindComponentByClass<UAbilitySystemComponent>();
		return IsValid(AbilitySystem);
	}

	AbilitySystem = nullptr;
	return false;
}

FGameplayTag UFuAbilitySystemUtility::GetFirstDescendantTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& ParentTag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(ParentTag.IsValid()))
	{
		return FGameplayTag::EmptyTag;
	}

	static FGameplayTagContainer Tags;
	check(Tags.IsEmpty())

	AbilitySystem->GetOwnedGameplayTags(Tags);

	const auto Tag{UFuGameplayTagUtility::GetFirstDescendantTag(Tags, ParentTag)};

	Tags.Reset();

	return Tag;
}

bool UFuAbilitySystemUtility::AddLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, const bool bReplicate)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	AbilitySystem->AddLooseGameplayTag(Tag);

	if (bReplicate)
	{
		AbilitySystem->AddReplicatedLooseGameplayTag(Tag);
	}

	return true;
}

bool UFuAbilitySystemUtility::RemoveLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, const bool bReplicate)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	AbilitySystem->RemoveLooseGameplayTag(Tag);

	if (bReplicate)
	{
		AbilitySystem->RemoveReplicatedLooseGameplayTag(Tag);
	}

	return true;
}

bool UFuAbilitySystemUtility::AddLooseTags(UAbilitySystemComponent* AbilitySystem,
                                           const FGameplayTagContainer& Tags, const bool bReplicate)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	AbilitySystem->AddLooseGameplayTags(Tags);

	if (bReplicate)
	{
		AbilitySystem->AddReplicatedLooseGameplayTags(Tags);
	}

	return true;
}

bool UFuAbilitySystemUtility::RemoveLooseTags(UAbilitySystemComponent* AbilitySystem,
                                              const FGameplayTagContainer& Tags, const bool bReplicate)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return false;
	}

	AbilitySystem->RemoveLooseGameplayTags(Tags);

	if (bReplicate)
	{
		AbilitySystem->RemoveReplicatedLooseGameplayTags(Tags);
	}

	return true;
}
