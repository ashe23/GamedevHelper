// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditor.h"
#include "Gdh.h"
#include "Actions/GdhNamingPolicyAssetActions.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

EAssetTypeCategories::Type FGdhEditor::NamingPolicyAssetCategory = EAssetTypeCategories::None;

void FGdhEditor::StartupModule()
{
	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>(GdhConstants::ModuleAssetTools).Get();

		NamingPolicyAssetCategory = AssetTools.RegisterAdvancedAssetCategory(GdhConstants::ModuleName, FText::FromName(GdhConstants::ModuleName));

		NamingPolicyAssetActions = MakeShared<FGdhNamingPolicyAssetActions>();
		AssetTools.RegisterAssetTypeActions(NamingPolicyAssetActions.ToSharedRef());
	}
}

void FGdhEditor::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(NamingPolicyAssetActions.ToSharedRef());
	}
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
