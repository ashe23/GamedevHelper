// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Gdh.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "LevelEditor.h"

DEFINE_LOG_CATEGORY(LogGdh);

void FGdh::StartupModule()
{
	IModuleInterface::StartupModule();

	FGdhStyles::Initialize();
	FGdhStyles::ReloadTextures();
	FGdhCommands::Register();

	Commands = MakeShareable(new FUICommandList);
	Commands->MapAction(
		FGdhCommands::Get().Cmd_RestartEditor,
		FExecuteAction::CreateLambda([]()
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		})
	);

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
					FText::FromString(GdhConstants::ModuleName.ToString()),
					FText::FromString("Open GamedevHelper Main Menu"),
					FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
					{
						MenuBuilder.BeginSection("GdhSectionEditor", FText::FromString("Editor"));
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_RestartEditor);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GdhSectionManager", FText::FromString("Managers"));
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenAssetNamingManager);
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenWorldOutlinearManager);
						MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenMrqBatchRenderManager);
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
