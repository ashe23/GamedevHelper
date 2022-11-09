// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhAssetNamingManagerListItem.generated.h"

UCLASS(Transient)
class UGdhAssetNamingManagerListItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FAssetData AssetData;
	
	UPROPERTY()
	FString OldName;
	
	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString Note;

	UPROPERTY()
	EGdhRenameStatus RenameStatus = EGdhRenameStatus::Ok;
};

class SGdhAssetNamingManagerListItem : public SMultiColumnTableRow<TWeakObjectPtr<UGdhAssetNamingManagerListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhAssetNamingManagerListItem) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhAssetNamingManagerListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
	
private:
	TWeakObjectPtr<UGdhAssetNamingManagerListItem> ListItem;
};
