// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhAssetNamingConvention;
class UGdhManagerAssetNamingItem;
class UGdhAssetScanSettings;

class SGdhManagerAssetNaming final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhManagerAssetNaming) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	void UpdateListData();
	void UpdateListView() const;
	void UpdateListSort(const FName& ColumnName);
	void OnSettingsChanged();
	void OnListSort(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type InSortMode);
	int32 GetWidgetIndex() const;
	TSharedRef<SWidget> CreateToolbarMain() const;
	TSharedRef<SHeaderRow> GetHeaderRow();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhManagerAssetNamingItem> Item, const TSharedRef<STableViewBase>& OwnerTable) const;
	void OnListRowMouseDoubleClick(TWeakObjectPtr<UGdhManagerAssetNamingItem> Item);
	FText GetListSummaryTxt() const;
	FText GetListSelectionTxt() const;
	FSlateColor GetListOptionsBtnForegroundColor() const;
	TSharedRef<SWidget> GetListOptionsBtnContent();

	bool bShowUnconfiguredAssets = false;
	TSharedPtr<FUICommandList> Cmds;
	TWeakObjectPtr<UGdhAssetScanSettings> ScanSettings;
	TWeakObjectPtr<UGdhAssetNamingConvention> AssetNamingConvention;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhManagerAssetNamingItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhManagerAssetNamingItem>> ListItems;
	TSharedPtr<SComboButton> ListOptionsBtn;

	EColumnSortMode::Type ColumnSortModeClass = EColumnSortMode::None;
	EColumnSortMode::Type ColumnSortModePath = EColumnSortMode::None;
};
