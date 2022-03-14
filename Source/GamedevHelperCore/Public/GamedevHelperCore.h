// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

namespace ModuleConstants
{
	const FName ModuleName{TEXT("GamedevHelperCore")};
}

class IGamedevHelperCore : public IModuleInterface
{
public:
	static IGamedevHelperCore& Get()
	{
		return FModuleManager::LoadModuleChecked<IGamedevHelperCore>(ModuleConstants::ModuleName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(ModuleConstants::ModuleName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogGamedevHelperCore, Log, All);