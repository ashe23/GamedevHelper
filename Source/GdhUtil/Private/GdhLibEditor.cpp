// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibEditor.h"
#include "GdhConstants.h"

FAssetToolsModule& UGdhLibEditor::GetModuleAssetTools()
{
	return FModuleManager::LoadModuleChecked<FAssetToolsModule>(GdhConstants::ModuleAssetTools);
}

FAssetRegistryModule& UGdhLibEditor::GetModuleAssetRegistry()
{
	return FModuleManager::LoadModuleChecked<FAssetRegistryModule>(GdhConstants::ModuleAssetRegistry);
}

FContentBrowserModule& UGdhLibEditor::GetModuleContentBrowser()
{
	return FModuleManager::LoadModuleChecked<FContentBrowserModule>(GdhConstants::ModuleContentBrowser);
}

FPropertyEditorModule& UGdhLibEditor::GetModulePropertyEditor()
{
	return FModuleManager::LoadModuleChecked<FPropertyEditorModule>(GdhConstants::ModulePropertyEditor);
}
