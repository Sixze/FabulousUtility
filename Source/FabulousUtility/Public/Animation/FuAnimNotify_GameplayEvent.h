#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FuAnimNotify_GameplayEvent.generated.h"

UCLASS(DisplayName = "Fu Gameplay Event Animation Notify")
class FABULOUSUTILITY_API UFuAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FGameplayTag EventTag;

public:
	UFuAnimNotify_GameplayEvent();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
