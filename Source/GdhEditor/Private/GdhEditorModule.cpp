// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorModule.h"
#include "AssetToolsModule.h"
#include "GdhConstants.h"
#include "CustomAssets/GdhRenderListActions.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

void FGdhEditorModule::StartupModule() {

	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools)) {
		const EAssetTypeCategories::Type Category = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
			GdhConstants::ModuleName, FText::FromName(GdhConstants::ModuleName)
		);

		RenderListActions = MakeShared<FGdhRenderListActions>(Category);

		FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(RenderListActions.ToSharedRef());
	}
}

void FGdhEditorModule::ShutdownModule() {

	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools)) {
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(RenderListActions.ToSharedRef());
	}
}

IMPLEMENT_MODULE(FGdhEditorModule, GdhEditorModule)
