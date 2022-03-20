// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "Widgets/SCompoundWidget.h"

class UGamedevHelperAssetNamingManagerSettings;
class UGamedevHelperAssetNamingListItem;

class SAssetNamingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetNamingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	void ListUpdate();
	void ListSort();
	void ListRefresh();
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGamedevHelperAssetNamingListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	FReply OnRenameBtnClick();
	FReply OnRefreshBtnClick();
	bool IsRenameBtnEnabled() const;
	void OnSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode);
	
	UGamedevHelperAssetNamingManagerSettings* Settings = nullptr;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>> ListView;
	TArray<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>> AssetList;
	TArray<FGamedevHelperRenamePreview> RenamePreviews;
	TEnumAsByte<EColumnSortMode::Type> CurrentSortMode = EColumnSortMode::Ascending;
	FName SortColumn;
};