#pragma once

#include "Modules/ModuleManager.h"

class FABULOUSEDITOR_API FFabulousEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};
