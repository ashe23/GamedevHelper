// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelper.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

class FGamedevHelper : public IGamedevHelper
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FGamedevHelper::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGamedevHelper::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelper, GamedevHelper)
DEFINE_LOG_CATEGORY(LogGamedevHelper);