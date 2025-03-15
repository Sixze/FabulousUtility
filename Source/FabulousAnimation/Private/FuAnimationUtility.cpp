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

bool UFuAnimationUtility::TryFindMontageNotifyEventByName(const UAnimMontage* Montage, const FName& NotifyName,
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

FTransform UFuAnimationUtility::GetBoneTransform(const UAnimMontage* Montage, const FName& BoneName, const float Time)
{
	if (!FU_ENSURE(IsValid(Montage)) || !FU_ENSURE(!Montage->SlotAnimTracks.IsEmpty()))
	{
		return FTransform::Identity;
	}

	auto ResultTransform{FTransform::Identity};

	const auto* Skeleton{Montage->GetSkeleton()};
	const auto& ReferenceSkeleton{Skeleton->GetReferenceSkeleton()};

	auto BoneIndex = ReferenceSkeleton.FindBoneIndex(BoneName);
	if (BoneIndex <= INDEX_NONE)
	{
		const auto* Socket{Skeleton->FindSocket(BoneName)};
		if (!IsValid(Socket))
		{
			return FTransform::Identity;
		}

		BoneIndex = ReferenceSkeleton.FindBoneIndex(Socket->BoneName);
		if (BoneIndex <= INDEX_NONE)
		{
			return FTransform::Identity;
		}

		ResultTransform = Socket->GetSocketLocalTransform();
	}

	const UAnimSequence* Sequence = nullptr;
	auto SequenceTime = 0.0f;

	for (const auto& SlotTrack : Montage->SlotAnimTracks)
	{
		const auto* Segment{SlotTrack.AnimTrack.GetSegmentAtTime(Time)};
		if (Segment == nullptr)
		{
			continue;
		}

		const auto* OtherSequence{Cast<UAnimSequence>(Segment->GetAnimReference())};
		if (IsValid(OtherSequence))
		{
			Sequence = OtherSequence;
			SequenceTime = Segment->ConvertTrackPosToAnimPos(Time);
			break;
		}
	}

	while (BoneIndex > 0)
	{
		auto LocalTransform{FTransform::Identity};
		Sequence->GetBoneTransform(LocalTransform, FSkeletonPoseBoneIndex{BoneIndex}, SequenceTime, false);

		ResultTransform *= LocalTransform;
		BoneIndex = ReferenceSkeleton.GetParentIndex(BoneIndex);
	}

	return ResultTransform;
}

FTransform UFuAnimationUtility::GetBoneTransformInComponentSpace(const FReferenceSkeleton& ReferenceSkeleton, int32 BoneIndex)
{
	if (!ReferenceSkeleton.IsValidIndex(BoneIndex))
	{
		return FTransform::Identity;
	}

	const auto& BonePoses{ReferenceSkeleton.GetRefBonePose()};
	const auto& BoneInfos{ReferenceSkeleton.GetRefBoneInfo()};

	auto ResultTransform{BonePoses[BoneIndex]};

	while (BoneIndex > 0)
	{
		const auto ParentBoneIndex{BoneInfos[BoneIndex].ParentIndex};

		ResultTransform *= BonePoses[ParentBoneIndex];
		BoneIndex = ParentBoneIndex;
	}

	return ResultTransform;
}

FTransform UFuAnimationUtility::GetBoneTransformInComponentSpace(const FBoneContainer& BoneContainer, FCompactPoseBoneIndex BoneIndex)
{
	if (BoneIndex >= BoneContainer.GetCompactPoseNumBones())
	{
		return FTransform::Identity;
	}

	auto ResultTransform{FTransform::Identity};

	while (BoneIndex.IsValid())
	{
		ResultTransform *= BoneContainer.GetRefPoseTransform(BoneIndex);
		BoneIndex = BoneContainer.GetParentBoneIndex(BoneIndex);
	}

	return ResultTransform;
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
