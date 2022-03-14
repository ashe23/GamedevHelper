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
	void RegisterContextMenu();
	void InitContextMenu(FMenuBuilder& MenuBuilder) const;
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

	// registering content browser context menu
	RegisterContextMenu();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGamedevHelperEditor::RegisterContextMenu));
	

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

	// unregistering context menu
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
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

void FGamedevHelperEditor::RegisterContextMenu()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu");
		FToolMenuSection& Section = Menu->AddSection("GamedevHelperActions", LOCTEXT("GamedevHelperActionsHeading", "GamedevHelper"));
		Section.InsertPosition = FToolMenuInsert("CommonAssetActions", EToolMenuInsertType::After);
		Section.AddSubMenu(
			"GamedevHelperSubMenu",
			LOCTEXT("GamedevHelperSubMenu", "GamedevHelper Actions"),
			LOCTEXT("GamedevHelperSubMenu_ToolTip", "GamedevHelper Helper Actions"),
			FNewMenuDelegate::CreateRaw(this, &FGamedevHelperEditor::InitContextMenu),
			false,
			FSlateIcon(FGamedevHelperEditorStyle::GetStyleSetName(), "GamedevHelper.Icon16")
		);
	}
}

void FGamedevHelperEditor::InitContextMenu(FMenuBuilder& MenuBuilder) const
{
	MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Animation Tool"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("VAT_StaticMesh", "StaticMesh"),
		LOCTEXT("VAT_StaticMesh_ToolTip", "Configure static mesh for vertex animation"),
		FSlateIcon(),
		FUIAction()
		// FUIAction(FExecuteAction::CreateStatic(&UVirtualGamesVertexAnimToolLibrary::ConfigureStaticMesh))
	);
	MenuBuilder.AddMenuEntry(
		LOCTEXT("VertexAnimTools_TextureNormal", "Texture Normal"),
		LOCTEXT("VertexAnimTools_TextureNormalToolTip", "Configure normal texture for vertex animation"),
		FSlateIcon(),
		FUIAction()
		// FUIAction(FExecuteAction::CreateStatic(&UVirtualGamesVertexAnimToolLibrary::ConfigureTexture, TextureNormal))
	);
	MenuBuilder.AddMenuEntry(
		LOCTEXT("VertexAnimTools_TextureUV", "Texture UV"),
		LOCTEXT("VertexAnimTools_TextureUVToolTip", "Configure UV texture for vertex animation"),
		FSlateIcon(),
		FUIAction()
		// FUIAction(FExecuteAction::CreateStatic(&UVirtualGamesVertexAnimToolLibrary::ConfigureTexture, TextureUV))
	);
	MenuBuilder.EndSection();
	
	MenuBuilder.BeginSection("Section_Utility", FText::FromString("Utility"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("Utility_DisableCollision", "Disable Collision"),
		LOCTEXT("Utility_DisableCollision_ToolTip", "Disable collision on selected static meshes"),
		FSlateIcon(),
		FUIAction()
		// FUIAction(FExecuteAction::CreateStatic(&UVirtualGamesAssetLibrary::DisableStaticMeshCollisions))
	);
	MenuBuilder.EndSection();
	
	MenuBuilder.BeginSection("Section_Naming", FText::FromString("Naming"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("Naming_FixName", "Fix Asset Name"),
		LOCTEXT("Naming_FixName_ToolTip", "Fixes selected asset name by convention"),
		FSlateIcon(),
		FUIAction()
		// FUIAction(FExecuteAction::CreateStatic(&UVirtualGamesLibrary::ProjectConventionFixSelectedAssetNames, SelectedAssets))
	);
	MenuBuilder.EndSection();
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
