// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

namespace ModuleConstants
{
	const FName ModuleName{TEXT("GamedevHelperEditor")};
}

class IGamedevHelperEditor : public IModuleInterface
{
public:
	static IGamedevHelperEditor& Get()
	{
		return FModuleManager::LoadModuleChecked<IGamedevHelperEditor>(ModuleConstants::ModuleName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(ModuleConstants::ModuleName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogGamedevHelperEditor, Log, All);