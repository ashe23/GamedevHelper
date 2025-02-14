// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetRenderListActions.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderListEdToolkit.h"

FGdhBetRenderListActions::FGdhBetRenderListActions(const EAssetTypeCategories::Type& InAssetTypeCategory) : Category(InAssetTypeCategory) {}

UClass* FGdhBetRenderListActions::GetSupportedClass() const {
	return UGdhBetRenderList::StaticClass();
}

FText FGdhBetRenderListActions::GetName() const {
	return FText::FromName("Gdh Render List");
}

FColor FGdhBetRenderListActions::GetTypeColor() const {
	return FColor::Black;
}

uint32 FGdhBetRenderListActions::GetCategories() {
	return Category;
}

void FGdhBetRenderListActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
	MakeShared<FGdhBetRenderListEdToolkit>()->InitEditor(InObjects);
}
