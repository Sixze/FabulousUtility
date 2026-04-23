#include "FuAnimationUtility.h"

#include "FuMacros.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Animation/Skeleton.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "Engine/SkeletalMeshSocket.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnimationUtility)

float UFuAnimationUtility::GetScaledPlayLength(const UAnimSequenceBase* Sequence)
{
	if (!FU_ENSURE(IsValid(Sequence)))
	{
		return 0.0f;
	}

	const auto PlayRate{FMath::Abs(Sequence->RateScale)};
	return PlayRate > UE_SMALL_NUMBER ? Sequence->GetPlayLength() / PlayRate : 0.0f;
}

bool UFuAnimationUtility::TryFindMontageNotifyEventByName(const UAnimMontage* Montage, const FName NotifyName,
                                                          FAnimNotifyEvent& NotifyEvent)
{
	if (!FU_ENSURE(IsValid(Montage)))
	{
		return false;
	}

	for (const auto& Notify : Montage->Notifies)
	{
		const auto* MontageNotify{Cast<UAnimNotify_PlayMontageNotify>(Notify.Notify)};
		if (IsValid(MontageNotify) && MontageNotify->GetNotifyName() == NotifyName)
		{
			NotifyEvent = Notify;
			return true;
		}

		const auto* MontageNotifyState{Cast<UAnimNotify_PlayMontageNotifyWindow>(Notify.NotifyStateClass)};
		if (IsValid(MontageNotifyState) && MontageNotifyState->GetNotifyName() == NotifyName)
		{
			NotifyEvent = Notify;
			return true;
		}
	}

	return false;
}

FTransform UFuAnimationUtility::GetBoneTransformFromMontage(const UAnimMontage* Montage, const FName BoneName, const float Time)
{
	if (!FU_ENSURE(IsValid(Montage)) || !FU_ENSURE(!Montage->SlotAnimTracks.IsEmpty()))
	{
		return FTransform::Identity;
	}

	for (const auto& SlotTrack : Montage->SlotAnimTracks)
	{
		const auto* Segment{SlotTrack.AnimTrack.GetSegmentAtTime(Time)};
		if (Segment == nullptr)
		{
			continue;
		}

		const auto* Sequence{Cast<UAnimSequence>(Segment->GetAnimReference())};
		if (IsValid(Sequence))
		{
			return GetBoneTransformFromSequence(Sequence, BoneName, Segment->ConvertTrackPosToAnimPos(Time));
		}
	}

	return FTransform::Identity;
}

FTransform UFuAnimationUtility::GetBoneTransformFromSequence(const UAnimSequence* Sequence, const FName BoneName, const float Time)
{
	auto ResultTransform{FTransform::Identity};

	if (!FU_ENSURE(IsValid(Sequence)))
	{
		return ResultTransform;
	}

	const auto* Skeleton{Sequence->GetSkeleton()};
	const auto& ReferenceSkeleton{Skeleton->GetReferenceSkeleton()};

	auto BoneIndex{ReferenceSkeleton.FindBoneIndex(BoneName)};

	if (!ReferenceSkeleton.IsValidIndex(BoneIndex))
	{
		const auto* Socket{Skeleton->FindSocket(BoneName)};
		if (!IsValid(Socket))
		{
			return ResultTransform;
		}

		BoneIndex = ReferenceSkeleton.FindBoneIndex(Socket->BoneName);
		if (!ReferenceSkeleton.IsValidIndex(BoneIndex))
		{
			return ResultTransform;
		}

		ResultTransform = Socket->GetSocketLocalTransform();
	}

	while (ReferenceSkeleton.IsValidIndex(BoneIndex))
	{
		auto BoneTransform{FTransform::Identity};
		const FAnimExtractContext ExtractionContext{static_cast<double>(Time)};

		Sequence->GetBoneTransform(BoneTransform, FSkeletonPoseBoneIndex{BoneIndex}, ExtractionContext, false);

		ResultTransform *= BoneTransform;
		BoneIndex = ReferenceSkeleton.GetParentIndex(BoneIndex);
	}

	return ResultTransform;
}

FTransform UFuAnimationUtility::GetReferenceBoneTransform(const FReferenceSkeleton& ReferenceSkeleton, const FName BoneName,
                                                          const FSkelMeshRefPoseOverride* ReferencePoseOverride)
{
	return GetReferenceBoneTransform(ReferenceSkeleton, ReferenceSkeleton.FindBoneIndex(BoneName), ReferencePoseOverride);
}

FTransform UFuAnimationUtility::GetReferenceBoneTransform(const FReferenceSkeleton& ReferenceSkeleton, int32 BoneIndex,
                                                          const FSkelMeshRefPoseOverride* ReferencePoseOverride)
{
	if (!ReferenceSkeleton.IsValidIndex(BoneIndex))
	{
		return FTransform::Identity;
	}

	const auto& BonePoses{ReferencePoseOverride != nullptr ? ReferencePoseOverride->RefBonePoses : ReferenceSkeleton.GetRefBonePose()};
	const auto& BoneInfos{ReferenceSkeleton.GetRefBoneInfo()};

	auto ResultTransform{BonePoses[BoneIndex]};
	BoneIndex = BoneInfos[BoneIndex].ParentIndex;

	while (BoneIndex >= 0)
	{
		ResultTransform *= BonePoses[BoneIndex];
		BoneIndex = BoneInfos[BoneIndex].ParentIndex;
	}

	return ResultTransform;
}

FTransform UFuAnimationUtility::GetReferenceBoneTransform(const FBoneContainer& BoneContainer, const FName BoneName)
{
	const auto MeshBoneIndex{BoneContainer.GetPoseBoneIndexForBoneName(BoneName)};
	const auto BoneIndex{BoneContainer.MakeCompactPoseIndex(FMeshPoseBoneIndex{MeshBoneIndex})};

	return GetReferenceBoneTransform(BoneContainer, BoneIndex);
}

FTransform UFuAnimationUtility::GetReferenceBoneTransform(const FBoneContainer& BoneContainer, FCompactPoseBoneIndex BoneIndex)
{
	if (!BoneIndex.IsValid() || BoneIndex >= BoneContainer.GetCompactPoseNumBones())
	{
		return FTransform::Identity;
	}

	auto ResultTransform{BoneContainer.GetRefPoseTransform(BoneIndex)};
	BoneIndex = BoneContainer.GetParentBoneIndex(BoneIndex);

	while (BoneIndex.IsValid())
	{
		ResultTransform *= BoneContainer.GetRefPoseTransform(BoneIndex);
		BoneIndex = BoneContainer.GetParentBoneIndex(BoneIndex);
	}

	return ResultTransform;
}

void UFuAnimationUtility::StopMontagesWithSlot(UAnimInstance* AnimationInstance, const FName SlotName, const float BlendOutDuration)
{
	if (!FU_ENSURE(IsValid(AnimationInstance)) || !FU_ENSURE(!SlotName.IsNone()))
	{
		return;
	}

	for (auto* MontageInstance : AnimationInstance->MontageInstances)
	{
		if (MontageInstance == nullptr || !MontageInstance->IsActive())
		{
			continue;
		}

		const auto* Montage{MontageInstance->Montage.Get()};

		for (const auto& SlotTrack : Montage->SlotAnimTracks)
		{
			if (SlotTrack.SlotName == SlotName)
			{
				continue;
			}

			FMontageBlendSettings BlendOutSettings{Montage->BlendOut};

			if (BlendOutDuration >= 0.0f)
			{
				BlendOutSettings.Blend.BlendTime = BlendOutDuration;
			}

			BlendOutSettings.BlendMode = Montage->BlendModeOut;
			BlendOutSettings.BlendProfile = Montage->BlendProfileOut;

			MontageInstance->Stop(BlendOutSettings);
			break;
		}
	}
}

void UFuAnimationUtility::StopMontagesWithAnySharedSlots(UAnimInstance* AnimationInstance, const UAnimMontage* ReferenceMontage,
                                                         const float BlendOutDuration)
{
	if (!FU_ENSURE(IsValid(AnimationInstance)) || !FU_ENSURE(IsValid(ReferenceMontage)))
	{
		return;
	}

	TSet<FName, DefaultKeyFuncs<FName>, TInlineSetAllocator<8>> SlotNames;
	SlotNames.Reserve(ReferenceMontage->SlotAnimTracks.Num());

	for (const auto& SlotTrack : ReferenceMontage->SlotAnimTracks)
	{
		SlotNames.Emplace(SlotTrack.SlotName);
	}

	for (auto* MontageInstance : AnimationInstance->MontageInstances)
	{
		if (MontageInstance == nullptr || !MontageInstance->IsActive())
		{
			continue;
		}

		const auto* Montage{MontageInstance->Montage.Get()};

		for (const auto& SlotTrack : Montage->SlotAnimTracks)
		{
			if (!SlotNames.Contains(SlotTrack.SlotName))
			{
				continue;
			}

			FMontageBlendSettings BlendOutSettings{Montage->BlendOut};

			if (BlendOutDuration >= 0.0f)
			{
				BlendOutSettings.Blend.BlendTime = BlendOutDuration;
			}

			BlendOutSettings.BlendMode = Montage->BlendModeOut;
			BlendOutSettings.BlendProfile = Montage->BlendProfileOut;

			MontageInstance->Stop(BlendOutSettings);
			break;
		}
	}
}
