#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FuAnimNotifyState_GameplayEvent.generated.h"

UCLASS(DisplayName = "Fu Gameplay Event Animation Notify State")
class FABULOUSANIMATION_API UFuAnimNotifyState_GameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FName NotifyName;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag BeginEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag EndEventTag;

public:
	UFuAnimNotifyState_GameplayEvent();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
	                         float Duration, const FAnimNotifyEventReference& NotifyEventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
	                       const FAnimNotifyEventReference& NotifyEventReference) override;
};
