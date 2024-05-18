#include "FuLatentActions.h"

#include "FuMacros.h"
#include "LatentActions.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuLatentActions)

class FABULOUSUTILITY_API FFuAdvancedDelayLatentAction : public FPendingLatentAction
{
protected:
	FName ExecutionFunction;
	int32 Linkage;
	FWeakObjectPtr CallbackTarget;

	float Duration;
	int32 LoopsCount;
	uint8 bStop : 1;

	float TimeRemaining;
	int32& LoopIndex;

	EFuAdvancedDelayOutputExec& OutputExec;

public:
	FFuAdvancedDelayLatentAction(const FLatentActionInfo& LatentInfo, float Duration, int32 LoopsCount,
	                             bool bSkipFirstDelay, int32& LoopIndex, EFuAdvancedDelayOutputExec& Output);

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	virtual FString GetDescription() const override;
#endif

public:
	void Retrigger(float NewDuration, int32 NewLoopsCount, bool bSkipFirstDelay);

	void Stop();
};

FFuAdvancedDelayLatentAction::FFuAdvancedDelayLatentAction(const FLatentActionInfo& LatentInfo, const float Duration,
                                                           const int32 LoopsCount, const bool bSkipFirstDelay, int32& LoopIndex,
                                                           EFuAdvancedDelayOutputExec& Output) : ExecutionFunction
	{LatentInfo.ExecutionFunction},
	Linkage{LatentInfo.Linkage},
	CallbackTarget{LatentInfo.CallbackTarget},
	Duration{FMath::Max(0.0f, Duration)},
	LoopsCount{FMath::Max(-1, LoopsCount)},
	bStop{false},
	TimeRemaining{this->Duration},
	LoopIndex{LoopIndex},
	OutputExec{Output}
{
	this->TimeRemaining = bSkipFirstDelay ? 0.0f : this->Duration;
	this->LoopIndex = -1;
	this->OutputExec = EFuAdvancedDelayOutputExec::OnLoop;
}

void FFuAdvancedDelayLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (bStop)
	{
		Response.DoneIf(true);
		return;
	}

	TimeRemaining -= Response.ElapsedTime();
	if (TimeRemaining > 0.0f)
	{
		return;
	}

	if (Duration <= UE_SMALL_NUMBER)
	{
		TimeRemaining = 0.0f;
		LoopIndex = FMath::Max(0, LoopsCount);
		OutputExec = EFuAdvancedDelayOutputExec::OnDelayEnded;

		Response.FinishAndTriggerIf(true, ExecutionFunction, Linkage, CallbackTarget);
		return;
	}

	const auto PassedLoops{FMath::TruncToInt32(-TimeRemaining / Duration) + 1};
	LoopIndex += PassedLoops;

	if (LoopsCount >= 0 && LoopIndex >= LoopsCount)
	{
		TimeRemaining = 0.0f;
		LoopIndex = LoopsCount;
		OutputExec = EFuAdvancedDelayOutputExec::OnDelayEnded;

		Response.FinishAndTriggerIf(true, ExecutionFunction, Linkage, CallbackTarget);
		return;
	}

	TimeRemaining += Duration * PassedLoops;

	Response.TriggerLink(ExecutionFunction, Linkage, CallbackTarget);
}

#if WITH_EDITOR
FString FFuAdvancedDelayLatentAction::GetDescription() const
{
	TStringBuilder<128> DescriptionBuilder{InPlace, TEXTVIEW("Time Remaining: ")};

	DescriptionBuilder.Appendf(TEXT("%.2f"), TimeRemaining);
	DescriptionBuilder << TEXTVIEW(", Loop Index: ") << LoopIndex;

	return FString{DescriptionBuilder};
}
#endif

void FFuAdvancedDelayLatentAction::Retrigger(const float NewDuration, const int32 NewLoopsCount, const bool bSkipFirstDelay)
{
	Duration = FMath::Max(0.0f, NewDuration);
	LoopsCount = FMath::Max(-1, NewLoopsCount);

	TimeRemaining = bSkipFirstDelay ? 0.0f : Duration;
	LoopIndex = -1;

	OutputExec = EFuAdvancedDelayOutputExec::OnLoop;
}

void FFuAdvancedDelayLatentAction::Stop()
{
	bStop = true;
}

void UFuLatentActions::AdvancedDelay(const UObject* WorldContext, const FLatentActionInfo LatentInfo,
                                     const EFuAdvancedDelayInputExec InputExec, const float Duration,
                                     const int32 LoopsCount, const bool bSkipFirstDelay, const bool bRetriggerable,
                                     int32& LoopIndex, EFuAdvancedDelayOutputExec& OutputExec)
{
	auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	if (!FU_ENSURE(IsValid(World)))
	{
		return;
	}

	auto& LatentActionManager{World->GetLatentActionManager()};
	auto* DelayAction{LatentActionManager.FindExistingAction<FFuAdvancedDelayLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID)};

	if (InputExec == EFuAdvancedDelayInputExec::Start)
	{
		if (DelayAction == nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
			                                 new FFuAdvancedDelayLatentAction{
				                                 LatentInfo, Duration, LoopsCount, bSkipFirstDelay, LoopIndex, OutputExec
			                                 });
		}
		else if (bRetriggerable)
		{
			DelayAction->Retrigger(Duration, LoopsCount, bSkipFirstDelay);
		}
	}
	else if (InputExec == EFuAdvancedDelayInputExec::Stop && DelayAction != nullptr)
	{
		DelayAction->Stop();
	}
}
