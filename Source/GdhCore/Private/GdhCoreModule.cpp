// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCoreModule.h"

DEFINE_LOG_CATEGORY(LogGdhCore);

void FGdhCoreModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGdhCoreModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhCoreModule, GdhCoreModule)
