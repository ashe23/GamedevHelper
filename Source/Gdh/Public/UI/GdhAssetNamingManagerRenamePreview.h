// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhAssetNamingManagerRenamePreview.generated.h"

USTRUCT()
struct FGdhRenamePreview
{
	GENERATED_BODY()

	bool IsValid() const;
	const FAssetData& GetAssetData() const;
	FString GetOldName() const;
	FString GetNewName() const;
	FString GetNewObjectPath() const;
	void SetStatus(const EGdhRenameStatus NewStatus);
	void SetAssetData(const FAssetData Asset);
	void SetNewName(const FString& Name);
	void SetNewObjectPath(const FString& Path);
	EGdhRenameStatus GetStatus() const;
	FString GetStatusMsg() const;

private:
	UPROPERTY()
	FAssetData AssetData;

	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString NewObjectPath;

	UPROPERTY()
	FString ErrMsg;

	UPROPERTY()
	EGdhRenameStatus RenameStatus = EGdhRenameStatus::Ok;
};
