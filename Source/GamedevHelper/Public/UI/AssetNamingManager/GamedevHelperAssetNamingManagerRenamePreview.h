// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetNamingManagerRenamePreview.generated.h"

USTRUCT()
struct FGamedevHelperRenamePreview
{
	GENERATED_BODY()

	bool IsValid() const;
	FAssetData GetAssetData() const;
	FString GetOldName() const;
	FString GetNewName() const;
	FString GetNewObjectPath() const;
	void SetStatus(const EGamedevHelperRenameStatus NewStatus);
	void SetAssetData(const FAssetData Asset);
	void SetNewName(const FString& Name);
	void SetNewObjectPath(const FString& Path);
	EGamedevHelperRenameStatus GetStatus() const;
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
	EGamedevHelperRenameStatus Status = EGamedevHelperRenameStatus::Ok;
};
