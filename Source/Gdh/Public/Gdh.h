// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdh, Log, All);

namespace GdhConstants
{
	// modules
	static const FName ModuleFullName{TEXT("GamedevHelper")};
	static const FName ModuleName{TEXT("Gdh")}; // Gdh - GamedevHelper
	static const FName ModuleStylesName{TEXT("GamedevHelperStyle")};
}

class FGdh final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;

private:
	TSharedPtr<FUICommandList> Commands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};
