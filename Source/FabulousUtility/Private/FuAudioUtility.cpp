#include "FuAudioUtility.h"

#include "AudioDevice.h"
#include "FuMacros.h"

void UFuAudioUtility::SetTransientMasterVolume(const UObject* WorldContext, const float Volume)
{
#if !UE_SERVER
	const auto* World{WorldContext->GetWorld()};
	auto* AudioDevice{FU_ENSURE(IsValid(World)) ? World->GetAudioDeviceRaw() : nullptr};

	if (AudioDevice != nullptr)
	{
		AudioDevice->SetTransientPrimaryVolume(Volume);
	}
#endif
}
