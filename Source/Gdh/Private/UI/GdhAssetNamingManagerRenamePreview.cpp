// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhAssetNamingManagerRenamePreview.h"

#include "Libs/GdhAssetLibrary.h"

bool FGdhRenamePreview::IsValid() const
{
	return RenameStatus == EGdhRenameStatus::OkToRename && AssetData.IsValid();
}

const FAssetData& FGdhRenamePreview::GetAssetData() const
{
	return AssetData;
}

FString FGdhRenamePreview::GetOldName() const
{
	if (!AssetData.IsValid()) return FString{};

	return AssetData.AssetName.ToString();
}

FString FGdhRenamePreview::GetNewName() const
{
	return NewName;
}

FString FGdhRenamePreview::GetNewObjectPath() const
{
	return NewObjectPath;
}

void FGdhRenamePreview::SetStatus(const EGdhRenameStatus NewStatus)
{
	RenameStatus = NewStatus;
}

void FGdhRenamePreview::SetAssetData(const FAssetData Asset)
{
	if (Asset.IsValid())
	{
		AssetData = Asset;
	}
}

void FGdhRenamePreview::SetNewName(const FString& Name)
{
	NewName = Name;
}

void FGdhRenamePreview::SetNewObjectPath(const FString& Path)
{
	NewObjectPath = Path;
}

EGdhRenameStatus FGdhRenamePreview::GetStatus() const
{
	return RenameStatus;
}

FString FGdhRenamePreview::GetStatusMsg() const
{
	if (RenameStatus == EGdhRenameStatus::MissingSettings)
	{
		if (AssetData.AssetClass.IsEqual(TEXT("Blueprint")))
		{
			if (const auto ParentClass = UGdhAssetLibrary::GetBlueprintParentClass(AssetData))
			{
				return FString::Printf(TEXT("Missing namings for %s"), *ParentClass->GetName());
			}
		}

		return FString::Printf(TEXT("Missing namings for %s"), *AssetData.GetClass()->GetName());
	}

	if (RenameStatus == EGdhRenameStatus::DuplicateNamePreview)
	{
		return TEXT("Asset with same name already exists in previews");
	}

	if (RenameStatus == EGdhRenameStatus::DuplicateNameContentBrowser)
	{
		return TEXT("Asset with same name already exists at this location in content browser");
	}

	if (RenameStatus == EGdhRenameStatus::OkToRename)
	{
		return TEXT("");
	}

	return TEXT("OK");
}
