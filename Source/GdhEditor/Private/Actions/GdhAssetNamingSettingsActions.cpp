// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Actions/GdhAssetNamingSettingsActions.h"
#include "Asset/GdhAssetNamingSettings.h"

UClass* FGdhAssetNamingSettingsActions::GetSupportedClass() const
{
	return UGdhAssetNamingSettings::StaticClass();
}

FText FGdhAssetNamingSettingsActions::GetName() const
{
	return INVTEXT("Asset Naming Settings");
}

FColor FGdhAssetNamingSettingsActions::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FGdhAssetNamingSettingsActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}
