// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

static const FName ModuleUtilityName{TEXT("ModuleUtility")};

class IModuleUtility : public IModuleInterface
{
public:
	static IModuleUtility& Get()
	{
		return FModuleManager::LoadModuleChecked<IModuleUtility>(ModuleUtilityName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(ModuleUtilityName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogModuleUtility, Log, All);
