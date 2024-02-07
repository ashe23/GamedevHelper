// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhToolsModule.h"

void FGdhToolsModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGdhToolsModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhToolsModule, GdhToolsModule)
