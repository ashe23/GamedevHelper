// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Gdh.h"
#include "GdhCommands.h"
#include "GdhStyles.h"
#include "UI/GdhRenderingManagerWindow.h"
// Engine Headers
#include "UnrealEdMisc.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Libs/GdhAssetLibrary.h"

IMPLEMENT_MODULE(FGdh, Gdh);
DEFINE_LOG_CATEGORY(LogGdh);

void FGdh::StartupModule()
{
	RegisterStyles();
	RegisterCommands();
	RegisterMainMenu();
	RegisterTabs();
	RegisterContextMenuActions();
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

void FGdh::RegisterContextMenuActions()
{
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.StaticMesh");
		FToolMenuSection& Section = Menu->FindOrAddSection("VSStaticMeshActions");
		Section.InsertPosition = FToolMenuInsert("CommonAssetActions", EToolMenuInsertType::After);
		Section.AddSubMenu(
			TEXT("GamedevHelperActionsSubMenu"),
			FText::FromString(TEXT("GamedevHelper Actions")),
			FText::FromString(TEXT("Asset Helper Actions")),
			FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Anim Tools"));
				MenuBuilder.AddMenuEntry(
					FText::FromString(TEXT("Configure for VertexAnim")),
					FText::FromString(TEXT("Configure selected static meshes for vertex animation")),
					FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.VertexAnim"),
					FUIAction(
						FExecuteAction::CreateLambda([]()
						{
							TArray<UStaticMesh*> StaticMeshes;
							UGdhAssetLibrary::GetSelectedAssetsFiltered<UStaticMesh>(StaticMeshes);
							UGdhAssetLibrary::VertexAnimConfigureStaticMeshes(StaticMeshes);
						})
					)
				);
				MenuBuilder.EndSection();
				MenuBuilder.BeginSection("Section_Util", FText::FromString("Utility"));
				MenuBuilder.AddMenuEntry(
					FText::FromString(TEXT("Disable Collision")),
					FText::FromString(TEXT("Disables collision on selected static meshes, included all LODS")),
					FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.Collision"),
					FUIAction(
						FExecuteAction::CreateLambda([]()
						{
							TArray<UStaticMesh*> StaticMeshes;
							UGdhAssetLibrary::GetSelectedAssetsFiltered<UStaticMesh>(StaticMeshes);
							UGdhAssetLibrary::DisableCollisions(StaticMeshes);
						})
					)
				);
				MenuBuilder.EndSection();
			}),
			false,
			FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon16")
		);
	}

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.Texture");
		FToolMenuSection& Section = Menu->FindOrAddSection("VSTextureActions");
		Section.InsertPosition = FToolMenuInsert("CommonAssetActions", EToolMenuInsertType::After);
		Section.AddSubMenu(
			TEXT("GamedevHelperActionsSubMenu"),
			FText::FromString(TEXT("GamedevHelper Actions")),
			FText::FromString(TEXT("Asset Helper Actions")),
			FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.BeginSection("Section_VAT", FText::FromString("Vertex Anim Tools"));
				MenuBuilder.AddMenuEntry(
					FText::FromString(TEXT("Configure for VertexAnim as UV")),
					FText::FromString(TEXT("Configure selected textures for vertex animation as UV texture")),
					FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.VertexAnim"),
					FUIAction(
						FExecuteAction::CreateLambda([]()
						{
							TArray<UTexture2D*> Textures;
							UGdhAssetLibrary::GetSelectedAssetsFiltered<UTexture2D>(Textures);
							UGdhAssetLibrary::VertexAnimConfigureTextures(Textures, EGdhVertexAnimTextureType::UV);
						})
					)
				);
				MenuBuilder.AddMenuEntry(
					FText::FromString(TEXT("Configure for VertexAnim as Normal")),
					FText::FromString(TEXT("Configure selected textures for vertex animation as Normal texture")),
					FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon.VertexAnim"),
					FUIAction(
						FExecuteAction::CreateLambda([]()
						{
							TArray<UTexture2D*> Textures;
							UGdhAssetLibrary::GetSelectedAssetsFiltered<UTexture2D>(Textures);
							UGdhAssetLibrary::VertexAnimConfigureTextures(Textures, EGdhVertexAnimTextureType::Normal);
						})
					)
				);
				MenuBuilder.EndSection();
			}),
			false,
			FSlateIcon(FGdhStyles::GetStyleSetName(), "GamedevHelper.Icon16")
		);
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
