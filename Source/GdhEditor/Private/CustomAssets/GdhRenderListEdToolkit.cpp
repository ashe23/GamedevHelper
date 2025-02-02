// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/GdhRenderListEdToolkit.h"
#include "CustomAssets/SGdhRenderList.h"
#include "GdhLibEditor.h"

void FGdhRenderListEdToolkit::InitEditor(const TArray<UObject*>& InObjects) {
	RenderList = Cast<UGdhRenderList>(InObjects[0]);   // TODO:ashe23 change later

	// clang-format off
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("GdhRenderListEditorLayout")
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
	    		->AddTab("GdhRenderListSequencesTab", ETabState::OpenedTab)
			)
	    )
	);

	// clang-format on

	FAssetEditorToolkit::InitAssetEditor(
		EToolkitMode::Standalone, {}, "GdhRenderListEditor", Layout, true, true, InObjects
	);
}

void FGdhRenderListEdToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory =
		InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Gdh Render List Editor"));

	// const FName TabIdDetails = TEXT("GdhRenderListDetailsTab");
	const FName TaIdList = TEXT("GdhRenderListSequencesTab");

	// InTabManager->RegisterTabSpawner(TabIdDetails, FOnSpawnTab::CreateRaw(this,
	// &FGdhRenderListEdToolkit::OnTabSpawnDetails)) 	.SetDisplayName(INVTEXT("Details"))
	// 	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager
		->RegisterTabSpawner(
			TaIdList, FOnSpawnTab::CreateRaw(this, &FGdhRenderListEdToolkit::OnTabSpawnSequences)
		)
		.SetDisplayName(INVTEXT("Sequences"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FGdhRenderListEdToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	// InTabManager->UnregisterTabSpawner("GdhRenderListDetailsTab");
	InTabManager->UnregisterTabSpawner("GdhRenderListSequencesTab");
}

FName FGdhRenderListEdToolkit::GetToolkitFName() const {
	return TEXT("GdhRenderListEditor");
}

FText FGdhRenderListEdToolkit::GetBaseToolkitName() const {
	return FText::FromString(TEXT("Gdh Render List Editor"));
}

FString FGdhRenderListEdToolkit::GetWorldCentricTabPrefix() const {
	return TEXT("GdhRenderList");
}

FLinearColor FGdhRenderListEdToolkit::GetWorldCentricTabColorScale() const {
	return FLinearColor::Blue;
}

TSharedRef<SDockTab> FGdhRenderListEdToolkit::OnTabSpawnSequences(const FSpawnTabArgs& Args) {
	return SNew(SDockTab)[SNew(SGdhRenderList).RenderList(RenderList)];
}

// TSharedRef<SDockTab> FGdhRenderListEdToolkit::OnTabSpawnDetails(const FSpawnTabArgs& Args) {
//
// 	FDetailsViewArgs DetailsViewArgs;
// 	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
//
// 	FPropertyEditorModule& ModuleProperty = UGdhLibEditor::GetModulePropertyEditor();
// 	TSharedRef<IDetailsView> DetailsView = ModuleProperty.CreateDetailView(DetailsViewArgs);
// 	DetailsView->SetObject(RenderList);
//
// 	return SNew(SDockTab)[DetailsView];
// }
