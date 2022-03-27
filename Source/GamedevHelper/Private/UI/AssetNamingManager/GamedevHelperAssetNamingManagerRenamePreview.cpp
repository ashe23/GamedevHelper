// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerRenamePreview.h"
#include "GamedevHelperAssetLibrary.h"

bool FGamedevHelperRenamePreview::IsValid() const
{
	return Status == EGamedevHelperRenameStatus::OkToRename && AssetData.IsValid();
}

FAssetData FGamedevHelperRenamePreview::GetAssetData() const
{
	return AssetData;
}

FString FGamedevHelperRenamePreview::GetOldName() const
{
	if (!AssetData.IsValid()) return FString{};

	return AssetData.AssetName.ToString();
}

FString FGamedevHelperRenamePreview::GetNewName() const
{
	return NewName;
}

FString FGamedevHelperRenamePreview::GetNewObjectPath() const
{
	return NewObjectPath;
}

void FGamedevHelperRenamePreview::SetStatus(const EGamedevHelperRenameStatus NewStatus)
{
	Status = NewStatus;
}

void FGamedevHelperRenamePreview::SetAssetData(const FAssetData Asset)
{
	if (Asset.IsValid())
	{
		AssetData = Asset;
	}
}

void FGamedevHelperRenamePreview::SetNewName(const FString& Name)
{
	NewName = Name;
}

void FGamedevHelperRenamePreview::SetNewObjectPath(const FString& Path)
{
	NewObjectPath = Path;
}

EGamedevHelperRenameStatus FGamedevHelperRenamePreview::GetStatus() const
{
	return Status;
}

FString FGamedevHelperRenamePreview::GetStatusMsg() const
{
	if (Status == EGamedevHelperRenameStatus::MissingSettings)
	{
		if (AssetData.AssetClass.IsEqual(TEXT("Blueprint")))
		{
			const auto ParentClass = UGamedevHelperAssetLibrary::GetBlueprintParentClass(AssetData);
			if (ParentClass)
			{
				return FString::Printf(TEXT("Missing namings for %s"), *ParentClass->GetName());
			}
		}

		return FString::Printf(TEXT("Missing namings for %s"), *AssetData.GetClass()->GetName());
	}

	if (Status == EGamedevHelperRenameStatus::DuplicateNamePreview)
	{
		return TEXT("Asset with same name already exists in previews");
	}

	if (Status == EGamedevHelperRenameStatus::DuplicateNameContentBrowser)
	{
		return TEXT("Asset with same name already exists at this location in content browser");
	}

	if (Status == EGamedevHelperRenameStatus::OkToRename)
	{
		return TEXT("");
	}

	return TEXT("OK");
}
