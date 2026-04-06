#pragma once

#include "BoneIndices.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAnimationUtility.generated.h"

struct FAnimNotifyEvent;
struct FReferenceSkeleton;
struct FSkelMeshRefPoseOverride;
struct FBoneContainer;
class UAnimSequenceBase;
class UAnimMontage;
class UAnimSequence;
class UAnimInstance;

UCLASS()
class FABULOUSANIMATION_API UFuAnimationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Animation|Utility", Meta = (ReturnDisplayName = "Play Length"))
	static float GetScaledPlayLength(const UAnimSequenceBase* Sequence);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Animation|Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool TryFindMontageNotifyEventByName(const UAnimMontage* Montage, FName NotifyName, FAnimNotifyEvent& NotifyEvent);

	UFUNCTION(BlueprintPure, Category = "Fabulous Animation|Utility", Meta = (ReturnDisplayName = "Transform"))
	static FTransform GetBoneTransformFromMontage(const UAnimMontage* Montage, FName BoneName, float Time);

	UFUNCTION(BlueprintPure, Category = "Fabulous Animation|Utility", Meta = (ReturnDisplayName = "Transform"))
	static FTransform GetBoneTransformFromSequence(const UAnimSequence* Sequence, FName BoneName, float Time);

	/// The result of the USkinnedMeshComponent::GetRefPoseOverride() function
	/// can be used as the ReferencePoseOverride argument in this function.
	static FTransform GetBoneTransformInComponentSpace(const FReferenceSkeleton& ReferenceSkeleton, FName BoneName,
	                                                   const FSkelMeshRefPoseOverride* ReferencePoseOverride = nullptr);

	/// The result of the USkinnedMeshComponent::GetRefPoseOverride() function
	/// can be used as the ReferencePoseOverride argument in this function.
	static FTransform GetBoneTransformInComponentSpace(const FReferenceSkeleton& ReferenceSkeleton, int32 BoneIndex,
	                                                   const FSkelMeshRefPoseOverride* ReferencePoseOverride = nullptr);

	static FTransform GetBoneTransformInComponentSpace(const FBoneContainer& BoneContainer, FName BoneName);

	static FTransform GetBoneTransformInComponentSpace(const FBoneContainer& BoneContainer, FCompactPoseBoneIndex BoneIndex);

	/// Stops all active animation montages that use the specified slot.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Animation|Utility")
	static void StopMontagesWithSlot(UAnimInstance* AnimationInstance, FName SlotName, float BlendOutDuration = -1.0f);

	/// Stops all active animation montages that share at least one slot with the reference montage.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Animation|Utility")
	static void StopMontagesWithAnySharedSlots(UAnimInstance* AnimationInstance, const UAnimMontage* ReferenceMontage,
	                                           float BlendOutDuration = -1.0f);
};
