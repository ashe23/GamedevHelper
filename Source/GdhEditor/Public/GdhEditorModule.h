// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdhEditor, Log, All);

class FGdhBetRenderListActions;

class FGdhEditorModule final : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void RegisterCmds();
	void RegisterTabs();
	void RegisterMainToolbar();
	void RegisterAssetContextMenu();
	void RegisterCustomAssets();
	void UnregisterCmds();
	void UnregisterTabs();
	void UnregisterCustomAssets();

	void OnRestartEditor() const;
	void OnOpenAnt();
	void OnOpenBet();
	TSharedRef<SDockTab> OnTabSpawnAnt(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> OnTabSpawnBet(const FSpawnTabArgs& Args) const;

	TSharedPtr<FUICommandList> Commands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
	TSharedPtr<FGdhBetRenderListActions> ActionsRenderList;
};
