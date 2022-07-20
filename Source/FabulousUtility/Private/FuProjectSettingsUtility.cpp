#include "FuProjectSettingsUtility.h"

#include "GeneralProjectSettings.h"

FString UFuProjectSettingsUtility::GetProjectVersion()
{
	return GetDefault<UGeneralProjectSettings>()->ProjectVersion;
}

FString UFuProjectSettingsUtility::GetCopyrightNotice()
{
	return GetDefault<UGeneralProjectSettings>()->CopyrightNotice;
}
