// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ModuleProjectOrganizer.h"
// #include "../Public/ModuleProjectOrganizer.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperModuleProjectOrganizer"

class FModuleProjectOrganizer : public IModuleProjectOrganizer
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FModuleProjectOrganizer::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModuleProjectOrganizer::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModuleProjectOrganizer, ModuleProjectOrganizer)
DEFINE_LOG_CATEGORY(LogModuleProjectOrganizer);