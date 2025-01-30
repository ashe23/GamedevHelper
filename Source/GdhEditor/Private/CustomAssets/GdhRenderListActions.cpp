// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/GdhRenderListActions.h"
#include "CustomAssets/GdhRenderList.h"
#include "CustomAssets/GdhRenderListEdToolkit.h"

FGdhRenderListActions::FGdhRenderListActions(const EAssetTypeCategories::Type& InAssetTypeCategory) :
	Category(InAssetTypeCategory) {}

UClass* FGdhRenderListActions::GetSupportedClass() const {
	return UGdhRenderList::StaticClass();
}

FText FGdhRenderListActions::GetName() const {
	return FText::FromName("Gdh Render List");
}

FColor FGdhRenderListActions::GetTypeColor() const {
	return FColor::Black;
}

uint32 FGdhRenderListActions::GetCategories() {
	return Category;
}

void FGdhRenderListActions::OpenAssetEditor(
	const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor
) {
	MakeShared<FGdhRenderListEdToolkit>()->InitEditor(InObjects);
}
