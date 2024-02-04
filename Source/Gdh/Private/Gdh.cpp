// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Gdh.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "LevelEditor.h"
#include "Slate/SGdhAssetNamingTool.h"
// #include "Slate/SGdhManagerAssetNaming.h"
// #include "Slate/SGdhManagerWorldOutlinear.h"

DEFINE_LOG_CATEGORY(LogGdh);

void FGdh::StartupModule()
{
	IModuleInterface::StartupModule();

	FGdhStyles::Initialize();
	FGdhStyles::ReloadTextures();
	FGdhCommands::Register();

	Commands = MakeShareable(new FUICommandList);
	Commands->MapAction(
		FGdhCommands::Get().RestartEditor,
		FExecuteAction::CreateLambda([]()
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		})
	);
	Commands->MapAction(
		FGdhCommands::Get().OpenAssetNamingTool,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabAssetNamingTool);
		})
	);

	// Commands->MapAction(
	// 	FGdhCommands::Get().Cmd_OpenWorldOutlinearManager,
	// 	FExecuteAction::CreateLambda([]()
	// 	{
	// 		FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabWorldOutlinearManager);
	// 	})
	// );
	//
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
	//
	// FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
	// 	                        GdhConstants::TabWorldOutlinearManager,
	// 	                        FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) -> TSharedRef<SDockTab>
	// 	                        {
	// 		                        return
	// 			                        SNew(SDockTab)
	// 			                        .TabRole(NomadTab)
	// 			                        [
	// 				                        SNew(SGdhManagerWorldOutlinear)
	// 			                        ];
	// 	                        })
	//                         )
	//                         .SetIcon(FGdhStyles::GetIcon("GamedevHelper.Tab.WorldOutlinearManager"))
	//                         .SetDisplayName(FText::FromName(TEXT("World Outliner Manager")))
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
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().RestartEditor);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GdhSectionTools", FText::FromString("Tools"));
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().OpenAssetNamingTool);
						// MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenWorldOutlinearManager);
						// MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenMrqBatchRenderingManager);
						MenuBuilder.EndSection();
					}),
					GdhConstants::ModuleName,
					FName(TEXT("GamedevHelperMenu"))
				);
			})
		);

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
}

void FGdh::ShutdownModule()
{
	FGdhStyles::Shutdown();
	FGdhCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabAssetNamingTool);
	// FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabWorldOutlinearManager);

	IModuleInterface::ShutdownModule();
}

bool FGdh::SupportsDynamicReloading()
{
	return false;
}

bool FGdh::IsGameModule() const
{
	return false;
}

IMPLEMENT_MODULE(FGdh, Gdh)
