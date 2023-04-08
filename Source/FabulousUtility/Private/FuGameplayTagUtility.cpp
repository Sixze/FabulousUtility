#include "FuGameplayTagUtility.h"

#include "GameplayTagsManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayTagUtility)

FName UFuGameplayTagUtility::GetSimpleTagName(const FGameplayTag& Tag)
{
	const auto TagNode{UGameplayTagsManager::Get().FindTagNode(Tag)};

	return TagNode.IsValid() ? TagNode->GetSimpleTagName() : NAME_None;
}

bool UFuGameplayTagUtility::HasChildTags(const FGameplayTag& Tag)
{
	const auto TagNode{UGameplayTagsManager::Get().FindTagNode(Tag)};

	return TagNode.IsValid() && !TagNode->GetChildTagNodes().IsEmpty();
}

FGameplayTagContainer UFuGameplayTagUtility::GetChildTagsWithoutDescendants(const FGameplayTag& Tag)
{
	const auto TagNode{UGameplayTagsManager::Get().FindTagNode(Tag)};
	if (!TagNode.IsValid())
	{
		return {};
	}

	FGameplayTagContainer Tags;

	for (const auto& ChildTagNode : TagNode->GetChildTagNodes())
	{
		if (ChildTagNode.IsValid())
		{
			Tags.AddTag(ChildTagNode->GetCompleteTag());
		}
	}

	return Tags;
}

FGameplayTag UFuGameplayTagUtility::GetFirstDescendantTag(const FGameplayTagContainer& Tags, const FGameplayTag& ParentTag)
{
	if (!ParentTag.IsValid())
	{
		return FGameplayTag::EmptyTag;
	}

	for (const auto& Tag : Tags)
	{
		if (Tag.MatchesTag(ParentTag))
		{
			return Tag;
		}
	}

	return FGameplayTag::EmptyTag;
}
