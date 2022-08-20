// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManager.h"
// Engine Headers
#include "IPythonScriptPlugin.h"

void FGdhRenderingManager::StartupModule()
{
	
}

void FGdhRenderingManager::ShutdownModule()
{
	
}

bool FGdhRenderingManager::SupportsDynamicReloading()
{
	return false;
}

bool FGdhRenderingManager::IsGameModule() const
{
	return false;
}

IMPLEMENT_MODULE(FGdhRenderingManager, GdhRenderingManager);
