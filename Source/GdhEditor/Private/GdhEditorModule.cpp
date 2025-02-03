// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorModule.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "LevelEditor.h"
#include "AssetToolsModule.h"
#include "ActorNamingTool/Slate/SGdhAnt.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderListActions.h"
#include "BatchEncodeTool/Slate/SGdhBet.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

void FGdhEditorModule::StartupModule() {
	FGdhStyles::Initialize();
	FGdhStyles::ReloadTextures();
	RegisterCmds();
	RegisterTabs();
	RegisterMainToolbar();
	RegisterAssetContextMenu();
	RegisterCustomAssets();
}

void FGdhEditorModule::ShutdownModule() {
	FGdhStyles::Shutdown();
	UnregisterCmds();
	UnregisterTabs();
	UnregisterCustomAssets();
}

void FGdhEditorModule::RegisterCmds() {
	FGdhCmds::Register();

	Commands = MakeShareable(new FUICommandList);
	Commands->MapAction(
		FGdhCmds::Get().RestartEditor,
		FExecuteAction::CreateRaw(this, &FGdhEditorModule::OnRestartEditor)
	);
	Commands->MapAction(
		FGdhCmds::Get().OpenActorNamingTool,
		FExecuteAction::CreateRaw(this, &FGdhEditorModule::OnOpenAnt)
	);
	Commands->MapAction(
		FGdhCmds::Get().OpenBatchEncodeTool,
		FExecuteAction::CreateRaw(this, &FGdhEditorModule::OnOpenBet)
	);
}

void FGdhEditorModule::RegisterTabs() {

	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			GdhConstants::TabActorNamingTool,
			FOnSpawnTab::CreateRaw(this, &FGdhEditorModule::OnTabSpawnAnt)
		)
		.SetIcon(FGdhStyles::GetIcon("GamedevHelper.Tab.ActorNamingTool"))
		.SetDisplayName(FText::FromName(TEXT("Actor Naming Tool")))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			GdhConstants::TabBatchEncodeTool,
			FOnSpawnTab::CreateRaw(this, &FGdhEditorModule::OnTabSpawnBet)
		)
		.SetIcon(FGdhStyles::GetIcon("GamedevHelper.Tab.BatchEncodeTool"))
		.SetDisplayName(FText::FromName(TEXT("Batch Encode Tool")))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGdhEditorModule::RegisterMainToolbar() {
	if (IsRunningCommandlet()) return;

	FLevelEditorModule& LevelEditorModule =
		FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
	MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuBarExtension(
		"Window",
		EExtensionHook::After,
		Commands,
		FMenuBarExtensionDelegate::CreateLambda([&](FMenuBarBuilder& MenuBarBuilder) {
			MenuBarBuilder.AddPullDownMenu(
				FText::FromString(GdhConstants::ModuleName.ToString()),
				FText::FromString("Open GamedevHelper Main Menu"),
				FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder) {
					MenuBuilder.BeginSection("GdhSectionEditor", FText::FromString("Editor"));
					MenuBuilder.AddMenuEntry(FGdhCmds::Get().RestartEditor);
					MenuBuilder.EndSection();

					MenuBuilder.BeginSection("GdhSectionTools", FText::FromString("Tools"));
					MenuBuilder.AddMenuEntry(FGdhCmds::Get().OpenActorNamingTool);
					MenuBuilder.AddMenuEntry(FGdhCmds::Get().OpenBatchEncodeTool);
					MenuBuilder.EndSection();
				}),
				GdhConstants::ModuleName,
				FName(TEXT("GamedevHelperMenu"))
			);
		})
	);

	LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
}

void FGdhEditorModule::RegisterAssetContextMenu() {
	// UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu");
	// FToolMenuSection& Section = Menu->FindOrAddSection("GdhActions");
	// Section.InsertPosition = FToolMenuInsert("CommonAssetActions", EToolMenuInsertType::After);
	// Section.AddSubMenu(
	// 	TEXT("GamedevHelperActionsSubMenu"),
	// 	FText::FromString(TEXT("Gdh Actions")),
	// 	FText::FromString(TEXT("Asset Helper Actions")),
	// 	FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
	// 	{
	// 		MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Anim Tools"));
	// 		MenuBuilder.AddMenuEntry(
	// 			FText::FromString(TEXT("Configure for VertexAnim")),
	// 			FText::FromString(TEXT("Configure selected static meshes for vertex animation")),
	// 			FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.VertexAnim"),
	// 			FUIAction(
	// 				FExecuteAction::CreateLambda([]()
	// 				{
	// 					UE_LOG(LogGdhTools, Warning, TEXT("Hello"));
	// 				})
	// 			)
	// 		);
	// 		MenuBuilder.EndSection();
	// 		MenuBuilder.BeginSection("Section_Util", FText::FromString("Utility"));
	// 		MenuBuilder.AddMenuEntry(
	// 			FText::FromString(TEXT("Disable Collision")),
	// 			FText::FromString(TEXT("Disables collision on selected static meshes, included all
	// LODS")), 			FSlateIcon(FGdhStyles::GetStyleSetName(),
	// "GamedevHelper.Icon.Collision"), 			FUIAction(
	// FExecuteAction::CreateLambda([]()
	// 				{
	// 					UE_LOG(LogGdhTools, Warning, TEXT("Hello"));
	// 				})
	// 			)
	// 		);
	// 		MenuBuilder.EndSection();
	// 	}),
	// 	false,
	// 	FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon16")
	// );

	// UToolMenus* ToolMenus = UToolMenus::Get();
	// UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.ActorContextMenu");
	// FToolMenuSection& Section = Menu->AddSection("GdhActions",
	// FText::FromName(TEXT("GdhActions"))); Section.AddMenuEntry(FGdhCmds::Get().RenameAssets);
}

void FGdhEditorModule::RegisterCustomAssets() {
	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools)) {
		const EAssetTypeCategories::Type Category =
			FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
				GdhConstants::ModuleName, FText::FromName(GdhConstants::ModuleName)
			);

		ActionsRenderList = MakeShared<FGdhBetRenderListActions>(Category);

		FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ActionsRenderList.ToSharedRef(
		));
	}
}

void FGdhEditorModule::UnregisterCmds() {
	FGdhCmds::Unregister();
}

void FGdhEditorModule::UnregisterTabs() {
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabActorNamingTool);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabBatchEncodeTool);
}

void FGdhEditorModule::UnregisterCustomAssets() {
	if (FModuleManager::Get().IsModuleLoaded(GdhConstants::ModuleAssetTools)) {
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			ActionsRenderList.ToSharedRef()
		);
	}
}

void FGdhEditorModule::OnRestartEditor() const {
	FUnrealEdMisc::Get().RestartEditor(true);
}

void FGdhEditorModule::OnOpenAnt() {
	FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabActorNamingTool);
}

void FGdhEditorModule::OnOpenBet() {
	FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabBatchEncodeTool);
}

TSharedRef<SDockTab> FGdhEditorModule::OnTabSpawnAnt(const FSpawnTabArgs& Args) const {
	return SNew(SDockTab).TabRole(NomadTab)[SNew(SGdhAnt)];
}

TSharedRef<SDockTab> FGdhEditorModule::OnTabSpawnBet(const FSpawnTabArgs& Args) const {
	return SNew(SDockTab).TabRole(NomadTab)[SNew(SGdhBet)];
}

IMPLEMENT_MODULE(FGdhEditorModule, GdhEditorModule)
