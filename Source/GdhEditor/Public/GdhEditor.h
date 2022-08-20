// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

namespace GdhEditorConstants
{
	static const FName ModuleName{TEXT("GdhEditor")};
}

class FGdhEditor : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;
private:
	static void RegisterStyles();
	void RegisterCommands();
	void RegisterMainMenu();
	static void UnregisterStyles();
	static void UnregisterCommands();
	// static void UnregisterMainMenu();

	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};

DECLARE_LOG_CATEGORY_EXTERN(LogGdhEditor, Log, All);