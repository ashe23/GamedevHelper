// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetEncodePresetEdToolkit.h"
#include "GdhLibEditor.h"

void FGdhBetEncodePresetEdToolkit::InitEditor(const TArray<UObject*>& InObjects) {
	EncodePreset = Cast<UGdhBetEncodePreset>(InObjects[0]);	  // TODO:ashe23 change later

	// clang-format off
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("GdhBetEncodePresetEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(1.0f)
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()->SetSizeCoefficient(1.0f)
				->AddTab("GdhBetEncodePresetTab", ETabState::OpenedTab)
			)
		)
	);

	// clang-format on

	FAssetEditorToolkit::InitAssetEditor(
		EToolkitMode::Standalone, {}, "GdhBetEncodePresetEditor", Layout, true, true, InObjects
	);
}

void FGdhBetEncodePresetEdToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager
) {
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory =
		InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Gdh Encode Preset Editor"));

	// const FName TabIdDetails = TEXT("GdhRenderListDetailsTab");
	const FName TaIdList = TEXT("GdhBetEncodePresetTab");

	// InTabManager->RegisterTabSpawner(TabIdDetails, FOnSpawnTab::CreateRaw(this,
	// &FGdhRenderListEdToolkit::OnTabSpawnDetails)) 	.SetDisplayName(INVTEXT("Details"))
	// 	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager
		->RegisterTabSpawner(
			TaIdList,
			FOnSpawnTab::CreateRaw(this, &FGdhBetEncodePresetEdToolkit::OnTabSpawnSequences)
		)
		.SetDisplayName(INVTEXT("Sequences"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FGdhBetEncodePresetEdToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager
) {
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner("GdhBetEncodePresetTab");
}

FName FGdhBetEncodePresetEdToolkit::GetToolkitFName() const {
	return TEXT("GdhBetEncodePresetEditor");
}
FText FGdhBetEncodePresetEdToolkit::GetBaseToolkitName() const {
	return FText::FromString(TEXT("Gdh Encode Preset Editor"));
}
FString FGdhBetEncodePresetEdToolkit::GetWorldCentricTabPrefix() const {
	return TEXT("GdhBetEncodePreset");
}
FLinearColor FGdhBetEncodePresetEdToolkit::GetWorldCentricTabColorScale() const {
	return FLinearColor::Blue;
}
TSharedRef<SDockTab> FGdhBetEncodePresetEdToolkit::OnTabSpawnSequences(const FSpawnTabArgs& Args) {
	// return SNew(SDockTab)[SNew(SGdhRenderList).RenderList(RenderList)];
	FPropertyEditorModule& ModuleProperty = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	TSharedRef<IDetailsView> DetailsView = ModuleProperty.CreateDetailView(DetailsViewArgs);

	return SNew(SDockTab);
}
