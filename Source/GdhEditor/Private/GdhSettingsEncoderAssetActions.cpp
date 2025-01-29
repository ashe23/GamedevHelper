// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSettingsEncoderAssetActions.h"
#include "GdhSettingsEncoder.h"

UClass* FGdhSettingsEncoderAssetActions::GetSupportedClass() const {
	return UGdhSettingsEncoder::StaticClass();
}

FText FGdhSettingsEncoderAssetActions::GetName() const {
	return INVTEXT("Gdh Video Encoder Settings");
}

FColor FGdhSettingsEncoderAssetActions::GetTypeColor() const {
	return FColor::Black;
}

uint32 FGdhSettingsEncoderAssetActions::GetCategories() {
	return EAssetTypeCategories::Misc;
}

// void FGdhSettingsEncoderAssetActions::OpenAssetEditor(
// 	const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor
// ) {
// 	// TODO:ashe23 implement later
// }
