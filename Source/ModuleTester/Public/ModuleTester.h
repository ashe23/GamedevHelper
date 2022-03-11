// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

static const FName ModuleTesterName{TEXT("ModuleTester")};

class IModuleTester : public IModuleInterface
{
public:
	static IModuleTester& Get()
	{
		return FModuleManager::LoadModuleChecked<IModuleTester>(ModuleTesterName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(ModuleTesterName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogModuleTester, Log, All);
