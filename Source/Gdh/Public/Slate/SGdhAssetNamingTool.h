// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhAssetNamingToolSettings;
class UGdhAssetNamingToolListItem;

class SGdhAssetNamingTool final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhAssetNamingTool) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	void UpdateListData();
	void UpdateListView() const;
	void UpdateListSort(const FName& ColumnName);
	void OnListSort(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type InSortMode);
	void OnListRowMouseDoubleClick(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item);
	FText GetListSummaryTxt() const;
	FText GetListSelectionTxt() const;
	FSlateColor GetListOptionsBtnForegroundColor() const;
	TSharedRef<SWidget> GetListOptionsBtnContent();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) const;
	TSharedRef<SWidget> CreateToolbarMain() const;
	TSharedRef<SHeaderRow> GetHeaderRow();
	int32 GetWidgetIndex() const;

	TSharedPtr<FUICommandList> Cmds;
	TSharedPtr<SComboButton> ListOptionsBtn;
	TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhAssetNamingToolListItem>>> ListView;
	TWeakObjectPtr<UGdhAssetNamingToolSettings> AssetNamingToolSettings;
	EColumnSortMode::Type ColumnSortModeClass = EColumnSortMode::None;
	EColumnSortMode::Type ColumnSortModePath = EColumnSortMode::None;
};
