// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperCore.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperCore"

class FGamedevHelperCore : public IGamedevHelperCore
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FGamedevHelperCore::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGamedevHelperCore::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelperCore, GamedevHelperCore)
DEFINE_LOG_CATEGORY(LogGamedevHelperCore);