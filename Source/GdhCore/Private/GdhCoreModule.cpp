// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCoreModule.h"

DEFINE_LOG_CATEGORY(LogGdh);

void FGdhCoreModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGdhCoreModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhCoreModule, GdhCoreModule)
