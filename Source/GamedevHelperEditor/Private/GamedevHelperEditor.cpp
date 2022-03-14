// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperEditor.h"
#include "GamedevHelperEditorStyle.h"
#include "GamedevHelperEditorCommands.h"
// Engine Headers
#include "LevelEditor.h"
#include "ToolMenus.h"


#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

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

}

void FGamedevHelperEditor::ShutdownModule()
{
	// unregistering styles
	FGamedevHelperEditorStyle::Shutdown();

	// unregistering commands
	FGamedevHelperEditorCommands::Unregister();
	
	// UToolMenus::UnRegisterStartupCallback(this);
	// UToolMenus::UnregisterOwner(this);
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
	UE_LOG(LogGamedevHelperEditor, Warning, TEXT("Openning some UI"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelperEditor, GamedevHelperEditor)
DEFINE_LOG_CATEGORY(LogGamedevHelperEditor);
