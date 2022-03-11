// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ModuleTester.h"

class FModuleTester : public IModuleTester
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


#define LOCTEXT_NAMESPACE "FGamedevHelperModuleTester"

void FModuleTester::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModuleTester::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModuleTester, ModuleTester)
DEFINE_LOG_CATEGORY(LogModuleTester);
