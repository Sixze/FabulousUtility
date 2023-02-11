#include "FuProjectSettingsUtility.h"

#include "GeneralProjectSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuProjectSettingsUtility)

FString UFuProjectSettingsUtility::GetProjectVersion()
{
	return GetDefault<UGeneralProjectSettings>()->ProjectVersion;
}

FString UFuProjectSettingsUtility::GetCopyrightNotice()
{
	return GetDefault<UGeneralProjectSettings>()->CopyrightNotice;
}
