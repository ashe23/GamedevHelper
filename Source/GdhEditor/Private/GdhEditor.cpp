// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditor.h"
#include "GdhEditorStyles.h"
#include "GdhEditorCommands.h"
#include "GdhRenderingManagerWindow.h"
// Engine Headers
#include "UnrealEdMisc.h"
#include "LevelEditor.h"
#include "ToolMenus.h"

void FGdhEditor::StartupModule()
{
	RegisterStyles();
	RegisterCommands();
	RegisterMainMenu();
	RegisterTabs();
}

void FGdhEditor::ShutdownModule()
{
	UnregisterStyles();
	UnregisterCommands();
	UnregisterTabs();
}

bool FGdhEditor::SupportsDynamicReloading()
{
	return false;
}

bool FGdhEditor::IsGameModule() const
{
	return false;
}

void FGdhEditor::RegisterStyles()
{
	FGdhEditorStyle::Initialize();
	FGdhEditorStyle::ReloadTextures();
}

void FGdhEditor::RegisterCommands()
{
	FGdhEditorCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGdhEditorCommands::Get().Cmd_RestartEditor,
		FExecuteAction::CreateLambda([]()
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		}),
		FCanExecuteAction()
	);
	PluginCommands->MapAction(
		FGdhEditorCommands::Get().Cmd_OpenRenderingManagerWindow,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GdhEditorConstants::TabRenderingManager);
		}),
		FCanExecuteAction()
	);
}

void FGdhEditor::RegisterMainMenu()
{
	if (!IsRunningCommandlet())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
		MenuExtender = MakeShareable(new FExtender);
		MenuExtender->AddMenuBarExtension(
			"Help",
			EExtensionHook::After,
			PluginCommands,
			FMenuBarExtensionDelegate::CreateLambda([&](FMenuBarBuilder& MenuBarBuilder)
			{
				MenuBarBuilder.AddPullDownMenu(
					FText::FromString("GamedevHelper"),
					FText::FromString("Open GamedevHelper Main Menu"),
					FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
					{
						MenuBuilder.BeginSection("GdhEditorSection", FText::FromString("Editor"));
						MenuBuilder.AddMenuEntry(FGdhEditorCommands::Get().Cmd_RestartEditor);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GdhManagersSection", FText::FromString("Managers"));
						// MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_AssetNamingManagerWindow);
						// MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_WorldOutlinerManagerWindow);
						MenuBuilder.AddMenuEntry(FGdhEditorCommands::Get().Cmd_OpenRenderingManagerWindow);
						MenuBuilder.EndSection();
					}),
					"GamedevHelper",
					FName(TEXT("GamedevHelperMenu"))
				);
			})
		);

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
}

void FGdhEditor::RegisterTabs()
{
	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			GdhEditorConstants::TabRenderingManager,
			FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& SpawnTabArgs)
			{
				return SNew(SDockTab).TabRole(MajorTab)[SNew(SGdhRenderingManagerWindow)];
			})
		)
		.SetDisplayName(FText::FromString(TEXT("Rendering Manager")))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FGdhEditorStyle::GetStyleSetName(), "GamedevHelper.Cmd_OpenRenderingManagerWindow"));
}

void FGdhEditor::UnregisterStyles()
{
	FGdhEditorStyle::Shutdown();
}

void FGdhEditor::UnregisterCommands()
{
	FGdhEditorCommands::Unregister();
}

void FGdhEditor::UnregisterTabs()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhEditorConstants::TabRenderingManager);
}

IMPLEMENT_MODULE(FGdhEditor, GdhEditor);
DEFINE_LOG_CATEGORY(LogGdhEditor);
