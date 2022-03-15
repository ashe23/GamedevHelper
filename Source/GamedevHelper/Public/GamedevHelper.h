// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

namespace GamedevHelperConstants
{
	static const FName ModuleName{TEXT("GamedevHelper")};
}

class IGamedevHelper : public IModuleInterface
{
public:
	static IGamedevHelper& Get()
	{
		return FModuleManager::LoadModuleChecked<IGamedevHelper>(GamedevHelperConstants::ModuleName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(GamedevHelperConstants::ModuleName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogGamedevHelper, Log, All);