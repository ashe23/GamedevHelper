// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectOrganizerAssetNamingListRow.generated.h"

UCLASS(Transient)
class UProjectOrganizerAssetNamingListRowData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FAssetData AssetData;
	UPROPERTY()
	FString OldName;
	UPROPERTY()
	FString NewName;
};


class SProjectOrganizerAssetNamingListRow : public SMultiColumnTableRow<TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>>
{
public:
	SLATE_BEGIN_ARGS(SProjectOrganizerAssetNamingListRow){}
		SLATE_ARGUMENT(TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>, RowItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
private:
	TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData> RowItem;
};