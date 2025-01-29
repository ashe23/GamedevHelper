// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/GdhVideoPipelineActions.h"
#include "CustomAssets/GdhVideoPipeline.h"

FGdhVideoPipelineActions::FGdhVideoPipelineActions(const EAssetTypeCategories::Type& InAssetTypeCategory) :
	Category(InAssetTypeCategory) {}

UClass* FGdhVideoPipelineActions::GetSupportedClass() const {
	return UGdhVideoPipeline::StaticClass();
}

FText FGdhVideoPipelineActions::GetName() const {
	return FText::FromName("Gdh Video Pipeline");
}

FColor FGdhVideoPipelineActions::GetTypeColor() const {
	return FColor::Black;
}

uint32 FGdhVideoPipelineActions::GetCategories() {
	return Category;
}
