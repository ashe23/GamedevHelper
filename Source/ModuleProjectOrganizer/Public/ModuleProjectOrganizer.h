// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

static const FName ModuleProjectOrganizerName{TEXT("ModuleProjectOrganizer")};

class IModuleProjectOrganizer : public IModuleInterface
{
public:
	static IModuleProjectOrganizer& Get()
	{
		return FModuleManager::LoadModuleChecked<IModuleProjectOrganizer>(ModuleProjectOrganizerName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(ModuleProjectOrganizerName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogModuleProjectOrganizer, Log, All);