// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorModule.h"
#include "AssetToolsModule.h"
#include "GdhConstants.h"
#include "CustomAssets/GdhVideoPipelineActions.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

void FGdhEditorModule::StartupModule() {

	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools)) {
		const EAssetTypeCategories::Type Category = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
			GdhConstants::ModuleName, FText::FromName(GdhConstants::ModuleFullName)
		);

		VideoPipelineActions = MakeShared<FGdhVideoPipelineActions>(Category);

		FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(VideoPipelineActions.ToSharedRef());
	}
}

void FGdhEditorModule::ShutdownModule() {

	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools)) {
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(VideoPipelineActions.ToSharedRef());
	}
}

IMPLEMENT_MODULE(FGdhEditorModule, GdhEditorModule)
