// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UProjectOrganizerAssetNamingListRowData;

class SProjectOrganizerAssetNamingUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SProjectOrganizerAssetNamingUI){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	void ListUpdate();
	void ListSort();
	
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	
	TSharedPtr<SListView<TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>>> ListView;
	TArray<TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>> AssetList;
	TEnumAsByte<EColumnSortMode::Type> CurrentSortMode = EColumnSortMode::Ascending;
	FName SortColumn;
};