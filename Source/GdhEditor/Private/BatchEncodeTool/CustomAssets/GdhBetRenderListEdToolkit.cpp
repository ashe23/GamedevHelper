// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetRenderListEdToolkit.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "BatchEncodeTool/Slate/SGdhBetRenderList.h"
#include "GdhConstants.h"

void FGdhBetRenderListEdToolkit::InitEditor(const TArray<UObject*>& InObjects) {

	if (!InObjects.IsValidIndex(0)) return;

	RenderList = Cast<UGdhBetRenderList>(InObjects[0]);

	// clang-format off
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("GdhBetRenderListLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(1.0f)
			->AddTab(GdhConstants::TabRenderList, ETabState::OpenedTab)
		)
	);
	// clang-format on

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "GdhBetRenderListEditor", Layout, true, true, InObjects);
}

void FGdhBetRenderListEdToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	const FText WorkspaceCategory = FText::FromString("GdhBet Render List Editor");
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(WorkspaceCategory);

	InTabManager->RegisterTabSpawner(GdhConstants::TabRenderList, FOnSpawnTab::CreateRaw(this, &FGdhBetRenderListEdToolkit::OnSpawnTabSequences))
		.SetDisplayName(INVTEXT("Render List"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FGdhBetRenderListEdToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(GdhConstants::TabRenderList);
}

FName FGdhBetRenderListEdToolkit::GetToolkitFName() const {
	return TEXT("GdhBetRenderListEditor");
}

FText FGdhBetRenderListEdToolkit::GetBaseToolkitName() const {
	return FText::FromString(TEXT("Gdh Bet Render List Editor"));
}

FString FGdhBetRenderListEdToolkit::GetWorldCentricTabPrefix() const {
	return TEXT("GdhBetRenderList");
}

FLinearColor FGdhBetRenderListEdToolkit::GetWorldCentricTabColorScale() const {
	return FLinearColor::Blue;
}

TSharedRef<SDockTab> FGdhBetRenderListEdToolkit::OnSpawnTabSequences(const FSpawnTabArgs& Args) {
	return SNew(SDockTab)[SNew(SGdhBetRenderList).RenderList(RenderList)];
}
