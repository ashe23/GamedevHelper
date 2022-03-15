// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelper.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetLibrary.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "UI/GamedevHelperEditorCommands.h"
// Engine Headers
#include "LevelEditor.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

static const FName GamedevHelperMainTabName{TEXT("GamedevHelperTab")};

class FGamedevHelper : public IGamedevHelper
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void RegisterCommands();
	void RegisterMainMenu();
	void RegisterContentBrowserContextMenu();
	void InitContentBrowserContextMenu(FMenuBuilder& MenuBuilder) const;
	void InitMainMenuBuilder(FMenuBarBuilder& MenuBarBuilder);
	void InitMainMenuEntries(FMenuBuilder& MenuBuilder) const;
	static void OnProjectOrganizerWindowClick();
	TSharedRef<SDockTab> OnMainTabClick(const FSpawnTabArgs& SpawnTabArgs) const;

	// actions
	static void OnContextMenuVATStaticMeshesClicked();
	static void OnContextMenuVatTexturesClicked(EGamedevHelperVertexAnimTexture TextureType);
	static void OnContextMenuDisableCollisionsClicked();

	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};

void FGamedevHelper::RegisterCommands()
{
	FGamedevHelperEditorCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGamedevHelperEditorCommands::Get().Cmd_ProjectOrganizerWindow,
		FExecuteAction::CreateStatic(&FGamedevHelper::OnProjectOrganizerWindowClick),
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
			"Window",
			EExtensionHook::After,
			PluginCommands,
			FMenuBarExtensionDelegate::CreateRaw(this, &FGamedevHelper::InitMainMenuBuilder));

		LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
	}
}

void FGamedevHelper::RegisterContentBrowserContextMenu()
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
			FNewMenuDelegate::CreateRaw(this, &FGamedevHelper::InitContentBrowserContextMenu),
			false,
			FSlateIcon(FGamedevHelperEditorStyle::GetStyleSetName(), "GamedevHelper.Icon16")
		);
	}
}

void FGamedevHelper::InitContentBrowserContextMenu(FMenuBuilder& MenuBuilder) const
{
	MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Animation Tool"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("VAT_StaticMesh", "StaticMesh"),
		LOCTEXT("VAT_StaticMesh_ToolTip", "Configure static meshes for vertex animation"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuVATStaticMeshesClicked))
	);
	MenuBuilder.AddMenuEntry(
		LOCTEXT("VAT_TextureNormal", "Texture Normal"),
		LOCTEXT("VAT_TextureNormalToolTip", "Configure normal textures for vertex animation"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuVatTexturesClicked, EGamedevHelperVertexAnimTexture::Normal))
	);
	MenuBuilder.AddMenuEntry(
		LOCTEXT("VAT_TextureUV", "Texture UV"),
		LOCTEXT("VAT_TextureUVToolTip", "Configure UV textures for vertex animation"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuVatTexturesClicked, EGamedevHelperVertexAnimTexture::UV))
	);
	MenuBuilder.EndSection();
	
	MenuBuilder.BeginSection("Section_Utility", FText::FromString("Utility"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("Utility_DisableCollision", "Disable Collision"),
		LOCTEXT("Utility_DisableCollision_ToolTip", "Disable collision on selected static meshes"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuDisableCollisionsClicked))
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

void FGamedevHelper::InitMainMenuBuilder(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		FText::FromString("GamedevHelper"),
		FText::FromString("Open GamedevHelper Menu"),
		FNewMenuDelegate::CreateRaw(this, &FGamedevHelper::InitMainMenuEntries),
		"GamedevHelper",
		FName(TEXT("GamedevHelperMenu"))
	);
}

void FGamedevHelper::InitMainMenuEntries(FMenuBuilder& MenuBuilder) const
{
	MenuBuilder.BeginSection("GDHCoreSection", FText::FromString("Project"));
	MenuBuilder.AddMenuEntry(FGamedevHelperEditorCommands::Get().Cmd_ProjectOrganizerWindow);
	MenuBuilder.EndSection();
}

void FGamedevHelper::OnProjectOrganizerWindowClick()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GamedevHelperMainTabName);
}

TSharedRef<SDockTab> FGamedevHelper::OnMainTabClick(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(MajorTab)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("AAAAA")))
		];
}

void FGamedevHelper::OnContextMenuVATStaticMeshesClicked()
{
	TArray<FAssetData> SelectedAssets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(SelectedAssets);

	TArray<UStaticMesh*> StaticMeshes;
	StaticMeshes.Reserve(SelectedAssets.Num());

	for (const auto& Asset: SelectedAssets)
	{
		UStaticMesh* StaticMesh = Cast<UStaticMesh>(Asset.GetAsset());
		if (!StaticMesh) continue;

		StaticMeshes.Add(StaticMesh);
	}

	UGamedevHelperAssetLibrary::VertexAnimConfigureStaticMeshes(StaticMeshes);
}

void FGamedevHelper::OnContextMenuVatTexturesClicked(EGamedevHelperVertexAnimTexture TextureType)
{
	TArray<FAssetData> SelectedAssets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(SelectedAssets);

	TArray<UTexture2D*> Textures;
	Textures.Reserve(SelectedAssets.Num());

	for (const auto& Asset : SelectedAssets)
	{
		UTexture2D* Texture = Cast<UTexture2D>(Asset.GetAsset());
		if (!Texture) continue;

		Textures.Add(Texture);
	}

	UGamedevHelperAssetLibrary::VertexAnimConfigureTextures(Textures, TextureType);
}

void FGamedevHelper::OnContextMenuDisableCollisionsClicked()
{
	TArray<FAssetData> SelectedAssets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(SelectedAssets);

	TArray<UStaticMesh*> StaticMeshes;
	StaticMeshes.Reserve(SelectedAssets.Num());

	for (const auto& Asset: SelectedAssets)
	{
		UStaticMesh* StaticMesh = Cast<UStaticMesh>(Asset.GetAsset());
		if (!StaticMesh) continue;

		StaticMeshes.Add(StaticMesh);
	}

	UGamedevHelperAssetLibrary::DisableCollisions(StaticMeshes);
}

void FGamedevHelper::StartupModule()
{
	FGamedevHelperEditorStyle::Initialize();
	FGamedevHelperEditorStyle::ReloadTextures();

	RegisterCommands();
	RegisterMainMenu();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGamedevHelper::RegisterContentBrowserContextMenu));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        GamedevHelperMainTabName,
		                        FOnSpawnTab::CreateRaw(this, &FGamedevHelper::OnMainTabClick)
	                        )
	                        .SetDisplayName(LOCTEXT("FGamedevHelperTabTitle", "ProjectOrganizer"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGamedevHelper::ShutdownModule()
{
	FGamedevHelperEditorStyle::Shutdown();
	FGamedevHelperEditorCommands::Unregister();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GamedevHelperMainTabName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelper, GamedevHelper)
DEFINE_LOG_CATEGORY(LogGamedevHelper);
