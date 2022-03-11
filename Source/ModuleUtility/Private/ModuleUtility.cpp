// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ModuleUtility.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperModuleUtility"

class FModuleUtility : public IModuleUtility
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FModuleUtility::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModuleUtility::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModuleUtility, ModuleUtility)
DEFINE_LOG_CATEGORY(LogModuleUtility);