// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperEditor.h"
#include "GamedevHelperEditorStyle.h"
#include "GamedevHelperEditorCommands.h"
#include "ProjectOrganizer/ProjectOrganizerWindow.h"
// Engine Headers
#include "LevelEditor.h"
#include "ToolMenus.h"


#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

static const FName GamedevHelperMainTabName{TEXT("GamedevHelperTab")};

class FGamedevHelperEditor : public IGamedevHelperEditor
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void RegisterCommands();
	void RegisterMainMenu();
	void InitMainMenuBuilder(FMenuBarBuilder& MenuBarBuilder);
	void InitMainMenuEntries(FMenuBuilder& MenuBarBuilder) const;
	static void OnProjectOrganizerWindowClick();
	TSharedRef<SDockTab> OnMainTabClick(const FSpawnTabArgs& SpawnTabArgs) const;

	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};

void FGamedevHelperEditor::StartupModule()
{
	// registering styles
	FGamedevHelperEditorStyle::Initialize();
	FGamedevHelperEditorStyle::ReloadTextures();

	// registering commands
	RegisterCommands();

	// registering main menu
	RegisterMainMenu();
	// UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGamedevHelperEditor::RegisterMainMenu));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        GamedevHelperMainTabName,
		                        FOnSpawnTab::CreateRaw(this, &FGamedevHelperEditor::OnMainTabClick)
	                        )
	                        .SetDisplayName(LOCTEXT("FGamedevHelperTabTitle", "ProjectOrganizer"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGamedevHelperEditor::ShutdownModule()
{
	// unregistering styles
	FGamedevHelperEditorStyle::Shutdown();

	// unregistering commands
	FGamedevHelperEditorCommands::Unregister();

	// UToolMenus::UnRegisterStartupCallback(this);
	// UToolMenus::UnregisterOwner(this);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GamedevHelperMainTabName);
}

void FGamedevHelperEditor::RegisterCommands()
{
	FGamedevHelperEditorCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGamedevHelperEditorCommands::Get().CMD_ProjectOrganizerWindow,
		FExecuteAction::CreateStatic(&FGamedevHelperEditor::OnProjectOrganizerWindowClick),
		FCanExecuteAction()
	);
}

void FGamedevHelperEditor::RegisterMainMenu()
{
	if (!IsRunningCommandlet())
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
		MenuExtender = MakeShareable(new FExtender);
		MenuExtender->AddMenuBarExtension(
			"Window",
			EExtensionHook::After,
			PluginCommands,
			FMenuBarExtensionDelegate::CreateRaw(this, &FGamedevHelperEditor::InitMainMenuBuilder));

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
}

void FGamedevHelperEditor::InitMainMenuBuilder(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		FText::FromString("GamedevHelper"),
		FText::FromString("Open GamedevHelper Menu"),
		FNewMenuDelegate::CreateRaw(this, &FGamedevHelperEditor::InitMainMenuEntries),
		"GamedevHelper",
		FName(TEXT("GamedevHelperMenu"))
	);
}

void FGamedevHelperEditor::InitMainMenuEntries(FMenuBuilder& MenuBarBuilder) const
{
	MenuBarBuilder.BeginSection("GDHCoreSection", FText::FromString("Project"));
	MenuBarBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().CMD_ProjectOrganizerWindow);
	MenuBarBuilder.EndSection();
}

void FGamedevHelperEditor::OnProjectOrganizerWindowClick()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GamedevHelperMainTabName);
}

TSharedRef<SDockTab> FGamedevHelperEditor::OnMainTabClick(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(MajorTab)
		[
			SNew(SProjectOrganizerWindow)
		];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelperEditor, GamedevHelperEditor)
DEFINE_LOG_CATEGORY(LogGamedevHelperEditor);
