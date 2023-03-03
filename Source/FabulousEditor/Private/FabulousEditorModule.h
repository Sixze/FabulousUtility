#pragma once

#include "Modules/ModuleInterface.h"

class FABULOUSEDITOR_API FFabulousEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};
