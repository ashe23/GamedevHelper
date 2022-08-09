// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelper.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetLibrary.h"
#include "GamedevHelperProjectSettings.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "UI/GamedevHelperEditorCommands.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerWindow.h"
#include "UI/WorldOutlinerManager/GamedevHelperWorldOutlinerManagerWindow.h"
#include "UI/Renderer/GamedevHelperRendererUI.h"
// Engine Headers
#include "GamedevHelperAssetNamingManagerLibrary.h"
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

	// actions
	static void OnContextMenuVatStaticMeshesClicked();
	static void OnContextMenuVatTexturesClicked(EGamedevHelperVertexAnimTexture TextureType);
	static void OnContextMenuDisableCollisionsClicked();
	static void OnContextMenuRenderSequencesClicked();
	static void OnContextMenuFixAssetNamesClicked();

	template <class ClassType>
	static bool CanExecuteActionOnSpecifiedAssets();

	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};

template <class ClassType>
bool FGamedevHelper::CanExecuteActionOnSpecifiedAssets()
{
	TArray<FAssetData> Assets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(Assets);

	for (const auto& Asset : Assets)
	{
		if (!Cast<ClassType>(Asset.GetAsset()))
		{
			return false;
		}
	}

	return true;
}

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

	// static mesh actions
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.StaticMesh");
		FToolMenuSection& Section = Menu->FindOrAddSection("GamedevHelperActions");

		Section.AddSubMenu(
			"GamedevHelperSubMenu",
			LOCTEXT("GamedevHelperSubMenu", "GamedevHelper Actions"),
			LOCTEXT("GamedevHelperSubMenu_ToolTip", "GamedevHelper plugin static mesh asset helper actions"),
			FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Animation Tools"));
				MenuBuilder.AddMenuEntry(
					LOCTEXT("VAT_StaticMesh", "Configure"),
					LOCTEXT("VAT_StaticMesh_ToolTip", "Configure selected static meshes for vertex animation"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuVatStaticMeshesClicked),
						FCanExecuteAction::CreateLambda([&]()
						{
							return CanExecuteActionOnSpecifiedAssets<UStaticMesh>();
						})
					)
				);
				MenuBuilder.EndSection();
				MenuBuilder.BeginSection("Section_Utility", FText::FromString("Utility"));
				MenuBuilder.AddMenuEntry(
					LOCTEXT("Utility_DisableCollision", "Disable Collision"),
					LOCTEXT("Utility_DisableCollision_ToolTip", "Disable collision on selected static meshes"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuDisableCollisionsClicked),
						FCanExecuteAction::CreateLambda([&]()
						{
							return CanExecuteActionOnSpecifiedAssets<UStaticMesh>();
						})
					)
				);
				MenuBuilder.EndSection();
			}),
			false,
			FSlateIcon(FGamedevHelperEditorStyle::GetStyleSetName(), "GamedevHelper.Icon16")
		);
	}

	// texture asset actions
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.Texture");
		FToolMenuSection& Section = Menu->FindOrAddSection("GamedevHelperActions");
		
		Section.AddSubMenu(
			"GamedevHelperSubMenu",
			LOCTEXT("GamedevHelperSubMenu", "GamedevHelper Actions"),
			LOCTEXT("GamedevHelperSubMenu_ToolTip", "GamedevHelper plugin asset helper actions"),
			FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Animation Tools"));
				MenuBuilder.AddMenuEntry(
					LOCTEXT("VAT_TextureNormal", "Configure as Normal"),
					LOCTEXT("VAT_TextureNormalToolTip", "Configure normal textures for vertex animation"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuVatTexturesClicked, EGamedevHelperVertexAnimTexture::Normal),
						FCanExecuteAction::CreateLambda([&]()
						{
							return CanExecuteActionOnSpecifiedAssets<UTexture2D>();
						})
					)
				);
				MenuBuilder.AddMenuEntry(
					LOCTEXT("VAT_TextureUV", "Configure as UV"),
					LOCTEXT("VAT_TextureUVToolTip", "Configure UV textures for vertex animation"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuVatTexturesClicked, EGamedevHelperVertexAnimTexture::UV),
						FCanExecuteAction::CreateLambda([&]()
						{
							return CanExecuteActionOnSpecifiedAssets<UTexture2D>();
						})
					)
				);
				MenuBuilder.EndSection();
			}),
			false,
			FSlateIcon(FGamedevHelperEditorStyle::GetStyleSetName(), "GamedevHelper.Icon16")
		);
	}

	// level sequence asset actions
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.LevelSequence");
		FToolMenuSection& Section = Menu->FindOrAddSection("GamedevHelperActions");
		
		Section.AddSubMenu(
			"GamedevHelperSubMenu",
			LOCTEXT("GamedevHelperSubMenu", "GamedevHelper Actions"),
			LOCTEXT("GamedevHelperSubMenu_ToolTip", "GamedevHelper plugin asset helper actions"),
			FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.BeginSection("Section_Util", FText::FromString("Utility"));
				MenuBuilder.AddMenuEntry(
					LOCTEXT("Util_Render", "Render"),
					LOCTEXT("Util_Render_ToolTip", "Renders specified level sequences using movie render and then creates video files from them"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&FGamedevHelper::OnContextMenuRenderSequencesClicked),
						FCanExecuteAction::CreateLambda([&]()
						{
							return CanExecuteActionOnSpecifiedAssets<ULevelSequence>();
						})
					)
				);
				MenuBuilder.EndSection();
			}),
			false,
			FSlateIcon(FGamedevHelperEditorStyle::GetStyleSetName(), "GamedevHelper.Icon16")
		);
	}
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

void FGamedevHelper::OnContextMenuVatStaticMeshesClicked()
{
	TArray<FAssetData> SelectedAssets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(SelectedAssets);

	TArray<UStaticMesh*> StaticMeshes;
	StaticMeshes.Reserve(SelectedAssets.Num());

	for (const auto& Asset : SelectedAssets)
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

	for (const auto& Asset : SelectedAssets)
	{
		UStaticMesh* StaticMesh = Cast<UStaticMesh>(Asset.GetAsset());
		if (!StaticMesh) continue;

		StaticMeshes.Add(StaticMesh);
	}

	UGamedevHelperAssetLibrary::DisableCollisions(StaticMeshes);
}

void FGamedevHelper::OnContextMenuRenderSequencesClicked()
{
	TArray<FAssetData> SelectedAssets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(SelectedAssets);

	TArray<ULevelSequence*> LevelSequences;

	for (const auto& Asset : SelectedAssets)
	{
		ULevelSequence* LevelSequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (!LevelSequence) continue;

		LevelSequences.Add(LevelSequence);
	}

	UE_LOG(LogGamedevHelper, Warning, TEXT("Level Sequences: %d"), LevelSequences.Num()); // todo:ashe23 show new popup window with basic settings
}

void FGamedevHelper::OnContextMenuFixAssetNamesClicked()
{
	TArray<FAssetData> SelectedAssets;
	UGamedevHelperAssetLibrary::GetSelectedAssets(SelectedAssets);
	UGamedevHelperAssetNamingManagerLibrary::RenameAssets(SelectedAssets);
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
