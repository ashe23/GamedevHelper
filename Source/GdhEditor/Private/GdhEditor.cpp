// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditor.h"
#include "Actions/GdhAssetNamingSettingsActions.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

void FGdhEditor::StartupModule()
{
	AssetNamingSettingsActions = MakeShared<FGdhAssetNamingSettingsActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(AssetNamingSettingsActions.ToSharedRef());
}

void FGdhEditor::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(AssetNamingSettingsActions.ToSharedRef());
}

bool FGdhEditor::SupportsDynamicReloading()
{
	return false;
}

bool FGdhEditor::IsGameModule() const
{
	return false;
}

IMPLEMENT_MODULE(FGdhEditor, GdhEditor)
