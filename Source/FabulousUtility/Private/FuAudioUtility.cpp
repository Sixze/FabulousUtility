#include "FuAudioUtility.h"

#include "AudioDevice.h"
#include "FuMacros.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAudioUtility)

void UFuAudioUtility::SetTransientPrimaryVolume(const UObject* WorldContext, const float Volume)
{
#if !UE_SERVER
	const auto* World{WorldContext->GetWorld()};
	auto* AudioDevice{IsValid(World) ? World->GetAudioDeviceRaw() : nullptr};

	if (AudioDevice != nullptr)
	{
		AudioDevice->SetTransientPrimaryVolume(Volume);
	}
#endif
}
