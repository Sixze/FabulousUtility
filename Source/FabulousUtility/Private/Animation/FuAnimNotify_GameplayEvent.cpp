#include "Animation/FuAnimNotify_GameplayEvent.h"

#include "AbilitySystem/Utility/FuAbilitySystemUtility.h"
#include "AbilitySystem/Utility/FuEventDataUtility.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnimNotify_GameplayEvent)

UFuAnimNotify_GameplayEvent::UFuAnimNotify_GameplayEvent()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
#endif

	bIsNativeBranchingPoint = true;
}

FString UFuAnimNotify_GameplayEvent::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("Fu Gameplay Event: {0}"), {EventTag.ToString()});
}

void UFuAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(Mesh, Animation, EventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		const auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};

		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(EventTag, &EventData);
	}
}
