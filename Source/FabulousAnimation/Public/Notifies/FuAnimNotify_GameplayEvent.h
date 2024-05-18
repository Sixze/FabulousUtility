#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FuAnimNotify_GameplayEvent.generated.h"

UCLASS(DisplayName = "Fu Gameplay Event Animation Notify")
class FABULOUSANIMATION_API UFuAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FName NotifyName;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FGameplayTag EventTag;

public:
	UFuAnimNotify_GameplayEvent();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
	                    const FAnimNotifyEventReference& NotifyEventReference) override;
};
