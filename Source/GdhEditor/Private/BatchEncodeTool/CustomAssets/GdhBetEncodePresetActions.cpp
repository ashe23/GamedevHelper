// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetEncodePresetActions.h"
#include "BatchEncodeTool/CustomAssets/GdhBetEncodePreset.h"

FGdhBetEncodePresetActions::FGdhBetEncodePresetActions(const EAssetTypeCategories::Type& InAssetTypeCategory) : Category(InAssetTypeCategory) {}

UClass* FGdhBetEncodePresetActions::GetSupportedClass() const {
	return UGdhBetEncodePreset::StaticClass();
}

FText FGdhBetEncodePresetActions::GetName() const {
	return FText::FromName("Gdh Encode Preset");
}

FColor FGdhBetEncodePresetActions::GetTypeColor() const {
	return FColor::Black;
}

uint32 FGdhBetEncodePresetActions::GetCategories() {
	return Category;
}

void FGdhBetEncodePresetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
	// MakeShared<FGdhRenderListEdToolkit>()->InitEditor(InObjects);
}
