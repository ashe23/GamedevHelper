// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelper.h"
#include "GamedevHelperProjectSettings.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "UI/GamedevHelperEditorCommands.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerWindow.h"
#include "UI/WorldOutlinerManager/GamedevHelperWorldOutlinerManagerWindow.h"
#include "UI/Renderer/GamedevHelperRendererUI.h"
// Engine Headers
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "UnrealEdMisc.h"
#include "ISettingsModule.h"
#include "LevelSequence.h"
#include "Developer/Settings/Public/ISettingsContainer.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

class FGamedevHelper : public IGamedevHelper
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void RegisterCommands();
	void RegisterMainMenu();
	void RegisterContentBrowserContextMenu();
	void RegisterProjectSettings() const;
	TSharedRef<SDockTab> OpenAssetNamingManagerWindow(const FSpawnTabArgs& SpawnTabArgs) const;
	TSharedRef<SDockTab> OpenWorldOutlinerManagerWindow(const FSpawnTabArgs& SpawnTabArgs) const;
	TSharedRef<SDockTab> OpenRendererWindow(const FSpawnTabArgs& SpawnTabArgs) const;

	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};

void FGamedevHelper::RegisterCommands()
{
	FGamedevHelperEditorCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGamedevHelperEditorCommands::Get().Cmd_AssetNamingManagerWindow,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GamedevHelperConstants::TabAssetNamingManager);
		}),
		FCanExecuteAction()
	);
	PluginCommands->MapAction(
		FGamedevHelperEditorCommands::Get().Cmd_WorldOutlinerManagerWindow,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GamedevHelperConstants::TabWorldOutlinerManager);
		}),
		FCanExecuteAction()
	);
	PluginCommands->MapAction(
		FGamedevHelperEditorCommands::Get().Cmd_EditorRestart,
		FExecuteAction::CreateLambda([]()
		{
			FUnrealEdMisc::Get().RestartEditor(true);
		}),
		FCanExecuteAction()
	);
	PluginCommands->MapAction(
		FGamedevHelperEditorCommands::Get().Cmd_RendererWindow,
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(GamedevHelperConstants::TabRendererWindow);
		}),
		FCanExecuteAction()
	);
}

void FGamedevHelper::RegisterMainMenu()
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
					FText::FromString("Open GamedevHelper Menu"),
					FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
					{
						MenuBuilder.BeginSection("GDHEditorSection", FText::FromString("Editor"));
						MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_EditorRestart);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GDHManagementSection", FText::FromString("Management"));
						MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_AssetNamingManagerWindow);
						MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_WorldOutlinerManagerWindow);
						MenuBuilder.EndSection();

						MenuBuilder.BeginSection("GHDRendererSection", FText::FromString("Rendering"));
						MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_RendererWindow);
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

void FGamedevHelper::RegisterContentBrowserContextMenu()
{
	FToolMenuOwnerScoped OwnerScoped(this);
}

void FGamedevHelper::RegisterProjectSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		const TSharedPtr<ISettingsContainer> ProjectSettingsContainer = SettingsModule->GetContainer("Project");
		ProjectSettingsContainer->DescribeCategory(
			TEXT("GamedevHelper"),
			FText::FromString("GamedevHelper"),
			FText::FromString("GamedevHelper Plugin Settings"));

		SettingsModule->RegisterSettings("Project", "GamedevHelper", "AssetNamingConventionSettings",
		                                 FText::FromString("Naming Convention"),
		                                 FText::FromString("Asset naming convention settings"),
		                                 GetMutableDefault<UGamedevHelperAssetNamingConventionSettings>()
		);
		SettingsModule->RegisterSettings("Project", "GamedevHelper", "WorldOutlinerSettings",
		                                 FText::FromString("World Outliner"),
		                                 FText::FromString("World outliner settings"),
		                                 GetMutableDefault<UGamedevHelperWorldOutlinerSettings>()
		);
	}
}

TSharedRef<SDockTab> FGamedevHelper::OpenAssetNamingManagerWindow(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(MajorTab)
		[
			SNew(SAssetNamingManagerWindow)
		];
}

TSharedRef<SDockTab> FGamedevHelper::OpenWorldOutlinerManagerWindow(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(MajorTab)
		[
			SNew(SWorldOutlinerManagerWindow)
		];
}

TSharedRef<SDockTab> FGamedevHelper::OpenRendererWindow(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(MajorTab)
		[
			SNew(SGamedevHelperRendererUI)
		];
}

void FGamedevHelper::StartupModule()
{
	FGamedevHelperEditorStyle::Initialize();
	FGamedevHelperEditorStyle::ReloadTextures();

	RegisterCommands();
	RegisterMainMenu();
	RegisterProjectSettings();

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGamedevHelper::RegisterContentBrowserContextMenu));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        GamedevHelperConstants::TabAssetNamingManager,
		                        FOnSpawnTab::CreateRaw(this, &FGamedevHelper::OpenAssetNamingManagerWindow)
	                        )
	                        .SetDisplayName(LOCTEXT("FGamedevHelperTabAssetNamingManager", "Asset Naming Manager"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        GamedevHelperConstants::TabWorldOutlinerManager,
		                        FOnSpawnTab::CreateRaw(this, &FGamedevHelper::OpenWorldOutlinerManagerWindow)
	                        )
	                        .SetDisplayName(LOCTEXT("FGamedevHelperTabWorlOutlinerManager", "World Outliner Manager"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        GamedevHelperConstants::TabRendererWindow,
		                        FOnSpawnTab::CreateRaw(this, &FGamedevHelper::OpenRendererWindow)
	                        )
	                        .SetDisplayName(LOCTEXT("FGamedevHelperTabRendererWindow", "Renderer"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGamedevHelper::ShutdownModule()
{
	FGamedevHelperEditorStyle::Shutdown();
	FGamedevHelperEditorCommands::Unregister();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GamedevHelperConstants::TabAssetNamingManager);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GamedevHelperConstants::TabWorldOutlinerManager);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GamedevHelperConstants::TabRendererWindow);

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "GamedevHelper", "GamedevHelperProjectSettings");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelper, GamedevHelper)
DEFINE_LOG_CATEGORY(LogGamedevHelper);
