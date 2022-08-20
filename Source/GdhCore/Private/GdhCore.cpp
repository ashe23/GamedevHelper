// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCore.h"

void FGdhCore::StartupModule()
{
	
}

void FGdhCore::ShutdownModule()
{
	
}

bool FGdhCore::SupportsDynamicReloading()
{
	return false;
}

bool FGdhCore::IsGameModule() const
{
	return false;
}

IMPLEMENT_MODULE(FGdhCore, GdhCore);
