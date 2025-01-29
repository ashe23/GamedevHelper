// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorModule.h"
#include "AssetToolsModule.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

void FGdhEditorModule::StartupModule() {
	SettingsEncoderAssetActions = MakeShared<FGdhSettingsEncoderAssetActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(SettingsEncoderAssetActions.ToSharedRef());
}

void FGdhEditorModule::ShutdownModule() {
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;

	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(SettingsEncoderAssetActions.ToSharedRef());
}

IMPLEMENT_MODULE(FGdhEditorModule, GdhEditorModule)
