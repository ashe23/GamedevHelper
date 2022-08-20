// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditor.h"
#include "GdhEditorStyles.h"
#include "GdhEditorCommands.h"
// Engine Headers
#include "UnrealEdMisc.h"
#include "LevelEditor.h"
#include "ToolMenus.h"

void FGdhEditor::StartupModule()
{
	RegisterStyles();
	RegisterCommands();
	RegisterMainMenu();
}

void FGdhEditor::ShutdownModule()
{
	UnregisterStyles();
	UnregisterCommands();
	// UnregisterMainMenu();
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

						// MenuBuilder.BeginSection("GDHManagementSection", FText::FromString("Management"));
						// MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_AssetNamingManagerWindow);
						// MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_WorldOutlinerManagerWindow);
						// MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_RenderingManagerWindow);
						// MenuBuilder.EndSection();
					}),
					"GamedevHelper",
					FName(TEXT("GamedevHelperMenu"))
				);
			})
		);

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
}

void FGdhEditor::UnregisterStyles()
{
	FGdhEditorStyle::Shutdown();
}

void FGdhEditor::UnregisterCommands()
{
	FGdhEditorCommands::Unregister();
}

// void FGdhEditor::UnregisterMainMenu()
// {
// }

IMPLEMENT_MODULE(FGdhEditor, GdhEditor);
DEFINE_LOG_CATEGORY(LogGdhEditor);
