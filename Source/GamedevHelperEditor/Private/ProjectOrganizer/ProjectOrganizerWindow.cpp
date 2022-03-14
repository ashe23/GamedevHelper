// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectOrganizer/ProjectOrganizerWindow.h"
#include "ProjectOrganizer/ProjectOrganizerAssetNamingUI.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

static const FName AssetNamingTab = FName{TEXT("AssetNamingTab")};
static const FName FolderStructureTab = FName{TEXT("FolderStructureTab")};

void SProjectOrganizerWindow::Construct(const FArguments& InArgs)
{
	InitTabs();
	ensure(TabManager.IsValid());

	const TSharedRef<SWidget> TabContents = TabManager->RestoreFrom(
		TabLayout.ToSharedRef(),
		TSharedPtr<SWindow>()
	).ToSharedRef();

	ChildSlot
	[
		TabContents
	];
}

SProjectOrganizerWindow::~SProjectOrganizerWindow()
{
	TabManager->UnregisterTabSpawner(AssetNamingTab);
	TabManager->UnregisterTabSpawner(FolderStructureTab);
}

void SProjectOrganizerWindow::InitTabs()
{
	const auto DummyTab = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	TabManager = FGlobalTabmanager::Get()->NewTabManager(DummyTab);
	TabManager->SetCanDoDragOperation(false);
	TabLayout = FTabManager::NewLayout("ProjectOrganizerTabLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab(AssetNamingTab, ETabState::OpenedTab)
				->AddTab(FolderStructureTab, ETabState::OpenedTab)
				->SetForegroundTab(AssetNamingTab)
			)
		);

	TabManager->RegisterTabSpawner(
		AssetNamingTab,
		FOnSpawnTab::CreateRaw(
			this,
			&SProjectOrganizerWindow::OnTabAssetNamingSpawn)
	);
	TabManager->RegisterTabSpawner(
		FolderStructureTab,
		FOnSpawnTab::CreateRaw(
			this,
			&SProjectOrganizerWindow::OnTabFolderStructureSpawn)
	);
}

TSharedRef<SDockTab> SProjectOrganizerWindow::OnTabAssetNamingSpawn(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		.Label(NSLOCTEXT("AssetNamingTab", "TabTitle", "Assets Naming"))
		.ShouldAutosize(true)
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.Padding(20.0f)
		[
			SNew(SProjectOrganizerAssetNamingUI)
		]
	];
}

TSharedRef<SDockTab> SProjectOrganizerWindow::OnTabFolderStructureSpawn(const FSpawnTabArgs& SpawnTabArgs) const
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		.Label(NSLOCTEXT("FolderStructureTab", "TabTitle", "Folder Structure"))
		.ShouldAutosize(true)
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.Padding(20.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("todo: Folder Structure UI here")))
		]
	];
}

#undef LOCTEXT_NAMESPACE
