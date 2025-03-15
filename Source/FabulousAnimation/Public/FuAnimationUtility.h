#pragma once

#include "BoneIndices.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAnimationUtility.generated.h"

struct FReferenceSkeleton;
struct FBoneContainer;

UCLASS()
class FABULOUSANIMATION_API UFuAnimationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Animation|Utility", Meta = (ReturnDisplayName = "Play Length"))
	static float GetScaledPlayLength(const UAnimSequenceBase* Sequence);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Animation|Utility",
		Meta = (AutoCreateRefTerm = "NotifyName", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryFindMontageNotifyEventByName(const UAnimMontage* Montage, const FName& NotifyName, FAnimNotifyEvent& NotifyEvent);

	UFUNCTION(BlueprintPure, Category = "Fabulous Animation|Utility",
		Meta = (AutoCreateRefTerm = "BoneName", ReturnDisplayName = "Transform"))
	static FTransform GetBoneTransform(const UAnimMontage* Montage, const FName& BoneName, float Time);

	static FTransform GetBoneTransformInComponentSpace(const FReferenceSkeleton& ReferenceSkeleton, int32 BoneIndex);

	static FTransform GetBoneTransformInComponentSpace(const FBoneContainer& BoneContainer, FCompactPoseBoneIndex BoneIndex);

	// Stops all active animation montages that share at least one slot with the reference montage.
	UFUNCTION(BlueprintCallable, Category="Fabulous Animation|Utility")
	static void StopMontagesWithAnySharedSlots(UAnimInstance* AnimationInstance, const UAnimMontage* ReferenceMontage,
	                                           float BlendOutDuration = -1.0f);
};
