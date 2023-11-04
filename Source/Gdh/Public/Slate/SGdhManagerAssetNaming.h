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

	TSharedRef<SWidget> CreateToolbarMain() const;
	TSharedRef<SHeaderRow> GetHeaderRow();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhManagerAssetNamingItem> Item, const TSharedRef<STableViewBase>& OwnerTable) const;

	TSharedPtr<FUICommandList> Cmds;
	TWeakObjectPtr<UGdhAssetScanSettings> ScanSettings;
	TWeakObjectPtr<UGdhAssetNamingConvention> AssetNamingConvention;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhManagerAssetNamingItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhManagerAssetNamingItem>> ListItems;
};
