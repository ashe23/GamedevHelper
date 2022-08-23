// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Gdh.h"
#include "GdhCommands.h"
#include "GdhStyles.h"
#include "UI/GdhRenderingManagerWindow.h"
// Engine Headers
#include "UnrealEdMisc.h"
#include "LevelEditor.h"

IMPLEMENT_MODULE(FGdh, Gdh);
DEFINE_LOG_CATEGORY(LogGdh);

void FGdh::StartupModule()
{
	RegisterStyles();
	RegisterCommands();
	RegisterMainMenu();
	RegisterTabs();
}

void FGdh::ShutdownModule()
{
	UnregisterStyles();
	UnregisterCommands();
	UnregisterTabs();
}

bool FGdh::SupportsDynamicReloading()
{
	return false;
}

bool FGdh::IsGameModule() const
{
	return false;
}

void FGdh::RegisterStyles()
{
	FGdhStyles::Initialize();
	FGdhStyles::ReloadTextures();
}

void FGdh::RegisterCommands()
{
	FGdhCommands::Register();
	Commands = MakeShareable(new FUICommandList);
	Commands->MapAction(
		FGdhCommands::Get().Cmd_RestartEditor,
		FExecuteAction::CreateLambda([]()
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		})
	);
	Commands->MapAction(
		FGdhCommands::Get().Cmd_OpenWindowRenderingManager,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GdhConstants::TabRenderingManager);
		})
	);
}

void FGdh::RegisterTabs()
{
	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			GdhConstants::TabRenderingManager,
			FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& SpawnTabArgs)
			{
				return
					SNew(SDockTab)
					.TabRole(MajorTab)
					[
						SNew(SGdhRenderingManagerWindow)
					];
			})
		)
		.SetDisplayName(FText::FromString(TEXT("Rendering Manager")))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Cmd_OpenWindowRenderingManager"));
}

void FGdh::RegisterMainMenu()
{
	if (!IsRunningCommandlet())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
		MenuExtender = MakeShareable(new FExtender);
		MenuExtender->AddMenuBarExtension(
			"Help",
			EExtensionHook::After,
			Commands,
			FMenuBarExtensionDelegate::CreateLambda([&](FMenuBarBuilder& MenuBarBuilder)
			{
				MenuBarBuilder.AddPullDownMenu(
					FText::FromString(GdhConstants::ModuleFullName.ToString()),
					FText::FromString("Open GamedevHelper Main Menu"),
					FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
					{
						MenuBuilder.BeginSection("GdhEditorSection", FText::FromString("Editor"));
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_RestartEditor);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GdhManagersSection", FText::FromString("Managers"));
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenWindowRenderingManager);
						MenuBuilder.EndSection();
					}),
					GdhConstants::ModuleFullName,
					FName(TEXT("GamedevHelperMenu"))
				);
			})
		);

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
}

void FGdh::UnregisterStyles()
{
	FGdhStyles::Shutdown();
}

void FGdh::UnregisterCommands()
{
	FGdhCommands::Unregister();
}

void FGdh::UnregisterTabs()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GdhConstants::TabRenderingManager);
}
