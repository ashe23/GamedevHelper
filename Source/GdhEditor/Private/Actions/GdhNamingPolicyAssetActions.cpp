// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Actions/GdhNamingPolicyAssetActions.h"

#include "GdhEditor.h"
#include "Asset/GdhNamingPolicyAsset.h"

UClass* FGdhNamingPolicyAssetActions::GetSupportedClass() const
{
	return UGdhNamingPolicyAsset::StaticClass();
}

FText FGdhNamingPolicyAssetActions::GetName() const
{
	return INVTEXT("Naming Policy Asset");
}

FColor FGdhNamingPolicyAssetActions::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FGdhNamingPolicyAssetActions::GetCategories()
{
	return FGdhEditor::NamingPolicyAssetCategory;
}
