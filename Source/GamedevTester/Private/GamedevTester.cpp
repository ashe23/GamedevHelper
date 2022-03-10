// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevTester.h"

DEFINE_LOG_CATEGORY(LogGamedevTester);

#define LOCTEXT_NAMESPACE "FGamedevTesterModule"

void FGamedevTesterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGamedevTesterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGamedevTesterModule, GamedevTester)