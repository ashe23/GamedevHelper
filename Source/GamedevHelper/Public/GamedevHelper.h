// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

namespace GamedevHelperConstants
{
	static const FName ModuleName = TEXT("GamedevHelper");
	static const FName TabAssetNamingManager{TEXT("TabAssetNamingManager")};
	static const FName TabWorldOutlinerManager{TEXT("TabWorldOutlinerManager")};
	static const FName TabRendererWindow{TEXT("TabRendererWindow")};
	static const FString AlphaLower = TEXT("abcdefghijklmnopqrstuvwxyz");
	static const FString AlphaUpper = AlphaLower.ToUpper();
	static const FString AlphaMixed = AlphaLower + AlphaLower.ToUpper();
	static const FString Digits = TEXT("0123456789");
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
