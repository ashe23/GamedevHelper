// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhUtilModule.h"

void FGdhUtilModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGdhUtilModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhUtilModule, GdhUtilModule)
