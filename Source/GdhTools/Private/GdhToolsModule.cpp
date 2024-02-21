// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhToolsModule.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "AssetNamingTool/Slate/SGdhAssetNamingTool.h"
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
	// Commands->MapAction(
	// 	FGdhCmds::Get().OpenAssetNamingTool,
	// 	FExecuteAction::CreateLambda([]()
	// 	{
	// 		FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabAssetNamingTool);
	// 	})
	// );
	//
	// FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
	// 	                        GdhConstants::TabAssetNamingTool,
	// 	                        FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) -> TSharedRef<SDockTab>
	// 	                        {
	// 		                        return
	// 			                        SNew(SDockTab)
	// 			                        .TabRole(MajorTab)
	// 			                        [
	// 				                        SNew(SGdhAssetNamingTool)
	// 			                        ];
	// 	                        })
	//                         )
	//                         .SetIcon(FGdhStyles::GetIcon("GamedevHelper.Tab.AssetNamingTool"))
	//                         .SetDisplayName(FText::FromName(TEXT("Asset Naming Tool")))
	//                         .SetMenuType(ETabSpawnerMenuType::Hidden);

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

						// MenuBuilder.BeginSection("GdhSectionTools", FText::FromString("Tools"));
						// MenuBuilder.AddMenuEntry(FGdhCmds::Get().OpenAssetNamingTool);
						// MenuBuilder.EndSection();
					}),
					GdhConstants::ModuleName,
					FName(TEXT("GamedevHelperMenu"))
				);
			})
		);

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}

	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.ActorContextMenu");
	FToolMenuSection& Section = Menu->AddSection("GdhActions", FText::FromName(TEXT("GdhActions")));
	Section.AddMenuEntry(FGdhCmds::Get().RenameAssets);
}

void FGdhToolsModule::ShutdownModule()
{
	FGdhStyles::Shutdown();
	FGdhCmds::Unregister();
	// FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabAssetNamingTool);
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhToolsModule, GdhToolsModule)
