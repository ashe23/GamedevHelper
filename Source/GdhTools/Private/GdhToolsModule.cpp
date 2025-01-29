// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhToolsModule.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "AssetNamingTool/Slate/SGdhAssetNamingTool.h"
#include "ActorNamingTool/Slate/SGdhActorNamingTool.h"
// Engine Headers
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "UnrealEdMisc.h"
#include "Widgets/Docking/SDockTab.h"

DEFINE_LOG_CATEGORY(LogGdhTools);

void FGdhToolsModule::StartupModule()
{
	IModuleInterface::StartupModule();

	FGdhStyles::Initialize();
	FGdhStyles::ReloadTextures();
	FGdhCmds::Register();

	Commands = MakeShareable(new FUICommandList);
	Commands->MapAction(
		FGdhCmds::Get().RestartEditor,
		FExecuteAction::CreateLambda([]()
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		})
	);
	Commands->MapAction(
		FGdhCmds::Get().RenameAssets,
		FExecuteAction::CreateLambda([]()
		{
			UE_LOG(LogGdhTools, Warning, TEXT("Hello"));
		})
	);
	Commands->MapAction(
		FGdhCmds::Get().OpenAssetNamingTool,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabAssetNamingTool);
		})
	);
	Commands->MapAction(
		FGdhCmds::Get().OpenActorNamingTool,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabActorNamingTool);
		})
	);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        GdhConstants::TabAssetNamingTool,
		                        FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) -> TSharedRef<SDockTab>
		                        {
			                        return
					                        SNew(SDockTab)
					                        .TabRole(MajorTab)
					                        [
						                        SNew(SGdhAssetNamingTool)
					                        ];
		                        })
	                        )
	                        .SetIcon(FGdhStyles::GetIcon("GamedevHelper.Tab.AssetNamingTool"))
	                        .SetDisplayName(FText::FromName(TEXT("Asset Naming Tool")))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
								GdhConstants::TabActorNamingTool,
								FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) -> TSharedRef<SDockTab>
								{
									return
											SNew(SDockTab)
											.TabRole(NomadTab)
											[
												SNew(SGdhActorNamingTool)
											];
								})
							)
							.SetIcon(FGdhStyles::GetIcon("GamedevHelper.Tab.ActorNamingTool"))
							.SetDisplayName(FText::FromName(TEXT("Actor Naming Tool")))
							.SetMenuType(ETabSpawnerMenuType::Hidden);

	if (!IsRunningCommandlet())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
		MenuExtender = MakeShareable(new FExtender);
		MenuExtender->AddMenuBarExtension(
			"Window",
			EExtensionHook::After,
			Commands,
			FMenuBarExtensionDelegate::CreateLambda([&](FMenuBarBuilder& MenuBarBuilder)
			{
				MenuBarBuilder.AddPullDownMenu(
					FText::FromString(GdhConstants::ModuleName.ToString()),
					FText::FromString("Open GamedevHelper Main Menu"),
					FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
					{
						MenuBuilder.BeginSection("GdhSectionEditor", FText::FromString("Editor"));
						MenuBuilder.AddMenuEntry(FGdhCmds::Get().RestartEditor);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GdhSectionTools", FText::FromString("Tools"));
						// MenuBuilder.AddMenuEntry(FGdhCmds::Get().OpenAssetNamingTool);
						MenuBuilder.AddMenuEntry(FGdhCmds::Get().OpenActorNamingTool);
						MenuBuilder.EndSection();
					}),
					GdhConstants::ModuleName,
					FName(TEXT("GamedevHelperMenu"))
				);
			})
		);

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu");
	FToolMenuSection& Section = Menu->FindOrAddSection("GdhActions");
	Section.InsertPosition = FToolMenuInsert("CommonAssetActions", EToolMenuInsertType::After);
	Section.AddSubMenu(
		TEXT("GamedevHelperActionsSubMenu"),
		FText::FromString(TEXT("Gdh Actions")),
		FText::FromString(TEXT("Asset Helper Actions")),
		FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Anim Tools"));
			MenuBuilder.AddMenuEntry(
				FText::FromString(TEXT("Configure for VertexAnim")),
				FText::FromString(TEXT("Configure selected static meshes for vertex animation")),
				FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.VertexAnim"),
				FUIAction(
					FExecuteAction::CreateLambda([]()
					{
						UE_LOG(LogGdhTools, Warning, TEXT("Hello"));
					})
				)
			);
			MenuBuilder.EndSection();
			MenuBuilder.BeginSection("Section_Util", FText::FromString("Utility"));
			MenuBuilder.AddMenuEntry(
				FText::FromString(TEXT("Disable Collision")),
				FText::FromString(TEXT("Disables collision on selected static meshes, included all LODS")),
				FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.Collision"),
				FUIAction(
					FExecuteAction::CreateLambda([]()
					{
						UE_LOG(LogGdhTools, Warning, TEXT("Hello"));
					})
				)
			);
			MenuBuilder.EndSection();
		}),
		false,
		FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon16")
	);

	// UToolMenus* ToolMenus = UToolMenus::Get();
	// UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.ActorContextMenu");
	// FToolMenuSection& Section = Menu->AddSection("GdhActions", FText::FromName(TEXT("GdhActions")));
	// Section.AddMenuEntry(FGdhCmds::Get().RenameAssets);
}

void FGdhToolsModule::ShutdownModule()
{
	FGdhStyles::Shutdown();
	FGdhCmds::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabAssetNamingTool);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabActorNamingTool);
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhToolsModule, GdhToolsModule)
