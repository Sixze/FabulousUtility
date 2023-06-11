#include "Utility/FuEventDataUtility.h"

#include "GameplayEffectExtension.h"
#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEventDataUtility)

FGameplayEventData UFuEventDataUtility::MakeEventDataFromEffectModificationData(const FGameplayEffectModCallbackData& Data)
{
	FGameplayEventData EventData;

	EventData.ContextHandle = Data.EffectSpec.GetContext();

	EventData.Instigator = EventData.ContextHandle.GetOriginalInstigator();

	const auto* InstigatorAbilitySystem{EventData.ContextHandle.GetInstigatorAbilitySystemComponent()};
	if (IsValid(InstigatorAbilitySystem))
	{
		InstigatorAbilitySystem->GetOwnedGameplayTags(EventData.InstigatorTags);
	}

	EventData.Target = Data.Target.GetOwnerActor();
	Data.Target.GetOwnedGameplayTags(EventData.TargetTags);

	return EventData;
}

FGameplayEventData UFuEventDataUtility::MakeEventDataFromAbilitySystems(const UAbilitySystemComponent* InstigatorAbilitySystem,
                                                                        const UAbilitySystemComponent* TargetAbilitySystem)
{
	FGameplayEventData EventData;

	if (IsValid(InstigatorAbilitySystem))
	{
		EventData.ContextHandle = InstigatorAbilitySystem->MakeEffectContext();

		EventData.Instigator = InstigatorAbilitySystem->GetOwnerActor();
		InstigatorAbilitySystem->GetOwnedGameplayTags(EventData.InstigatorTags);
	}

	if (IsValid(TargetAbilitySystem))
	{
		EventData.Target = TargetAbilitySystem->GetOwnerActor();
		TargetAbilitySystem->GetOwnedGameplayTags(EventData.TargetTags);
	}

	return EventData;
}

FGameplayEventData UFuEventDataUtility::MakeEventDataFromAbilitySystemAndAvatar(const UAbilitySystemComponent* InstigatorAbilitySystem,
                                                                                const AActor* TargetAvatar)
{
	FGameplayEventData EventData;

	if (IsValid(InstigatorAbilitySystem))
	{
		EventData.ContextHandle = InstigatorAbilitySystem->MakeEffectContext();

		EventData.Instigator = InstigatorAbilitySystem->GetOwnerActor();
		InstigatorAbilitySystem->GetOwnedGameplayTags(EventData.InstigatorTags);
	}

	UAbilitySystemComponent* TargetAbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(TargetAvatar, TargetAbilitySystem))
	{
		EventData.Target = TargetAbilitySystem->GetOwnerActor();
		TargetAbilitySystem->GetOwnedGameplayTags(EventData.TargetTags);
	}
	else
	{
		EventData.Target = TargetAvatar;
	}

	return EventData;
}

FGameplayEventData UFuEventDataUtility::MakeEventDataFromAvatarAndAbilitySystem(const AActor* InstigatorAvatar,
                                                                                const UAbilitySystemComponent* TargetAbilitySystem)
{
	FGameplayEventData EventData;

	UAbilitySystemComponent* InstigatorAbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(InstigatorAvatar, InstigatorAbilitySystem))
	{
		EventData.ContextHandle = InstigatorAbilitySystem->MakeEffectContext();

		EventData.Instigator = InstigatorAbilitySystem->GetOwnerActor();
		InstigatorAbilitySystem->GetOwnedGameplayTags(EventData.InstigatorTags);
	}
	else
	{
		EventData.Instigator = InstigatorAvatar;
	}

	if (IsValid(TargetAbilitySystem))
	{
		EventData.Target = TargetAbilitySystem->GetOwnerActor();
		TargetAbilitySystem->GetOwnedGameplayTags(EventData.TargetTags);
	}

	return EventData;
}

FGameplayEventData UFuEventDataUtility::MakeEventDataFromActorInfos(const FGameplayAbilityActorInfo& InstigatorActorInfo,
                                                                    const FGameplayAbilityActorInfo& TargetActorInfo)
{
	FGameplayEventData EventData;

	if (InstigatorActorInfo.AbilitySystemComponent.IsValid())
	{
		EventData.ContextHandle = InstigatorActorInfo.AbilitySystemComponent->MakeEffectContext();

		EventData.Instigator = InstigatorActorInfo.OwnerActor.Get();
		InstigatorActorInfo.AbilitySystemComponent->GetOwnedGameplayTags(EventData.InstigatorTags);
	}

	if (TargetActorInfo.AbilitySystemComponent.IsValid())
	{
		EventData.Target = TargetActorInfo.OwnerActor.Get();
		TargetActorInfo.AbilitySystemComponent->GetOwnedGameplayTags(EventData.TargetTags);
	}

	return EventData;
}

FGameplayEventData UFuEventDataUtility::MakeEventDataFromActorInfoAndAvatar(const FGameplayAbilityActorInfo& InstigatorActorInfo,
                                                                            const AActor* TargetAvatar)
{
	FGameplayEventData EventData;

	if (InstigatorActorInfo.AbilitySystemComponent.IsValid())
	{
		EventData.ContextHandle = InstigatorActorInfo.AbilitySystemComponent->MakeEffectContext();

		EventData.Instigator = InstigatorActorInfo.OwnerActor.Get();
		InstigatorActorInfo.AbilitySystemComponent->GetOwnedGameplayTags(EventData.InstigatorTags);
	}

	UAbilitySystemComponent* TargetAbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(TargetAvatar, TargetAbilitySystem))
	{
		EventData.Target = TargetAbilitySystem->GetOwnerActor();
		TargetAbilitySystem->GetOwnedGameplayTags(EventData.TargetTags);
	}
	else
	{
		EventData.Target = TargetAvatar;
	}

	return EventData;
}

FGameplayEventData UFuEventDataUtility::MakeEventDataFromAvatars(const AActor* InstigatorAvatar, const AActor* TargetAvatar)
{
	FGameplayEventData EventData;

	UAbilitySystemComponent* InstigatorAbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(InstigatorAvatar, InstigatorAbilitySystem))
	{
		EventData.ContextHandle = InstigatorAbilitySystem->MakeEffectContext();

		EventData.Instigator = InstigatorAbilitySystem->GetOwnerActor();
		InstigatorAbilitySystem->GetOwnedGameplayTags(EventData.InstigatorTags);
	}

	UAbilitySystemComponent* TargetAbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(TargetAvatar, TargetAbilitySystem))
	{
		EventData.Target = TargetAbilitySystem->GetOwnerActor();
		TargetAbilitySystem->GetOwnedGameplayTags(EventData.TargetTags);
	}
	else
	{
		EventData.Target = TargetAvatar;
	}

	return EventData;
}
