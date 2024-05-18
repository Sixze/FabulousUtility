#include "Notifies/FuAnimNotify_GameplayEvent.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/FuAbilitySystemUtility.h"
#include "Utility/FuEventDataUtility.h"

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
#if WITH_EDITOR
	if (!NotifyName.IsNone())
	{
		return NotifyName.ToString();
	}
#endif

	TStringBuilder<256> NotifyNameBuilder{InPlace, TEXTVIEW("Fu Gameplay Event: "), EventTag.GetTagName()};

	return FString{NotifyNameBuilder};
}

void UFuAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                         const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::Notify(Mesh, Sequence, NotifyEventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
		EventData.OptionalObject = Sequence;

		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(EventTag, &EventData);
	}
}
