// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCoreModule.h"

void FGdhCoreModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGdhCoreModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhCoreModule, GdhCoreModule)
