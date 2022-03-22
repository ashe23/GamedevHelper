// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "Widgets/SCompoundWidget.h"

class UGamedevHelperAssetNamingManagerSettings;
class UGamedevHelperAssetNamingConvention;
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
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGamedevHelperAssetNamingListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const;
	TSharedPtr<SWidget> GetListContextMenu() const;
	void OnListSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode);
	FReply OnRenameBtnClick();
	void OnRenameSelected();
	FReply OnRefreshBtnClick();
	bool IsRenameBtnEnabled() const;

	TSharedPtr<FUICommandList> PluginCommands;
	UGamedevHelperAssetNamingManagerSettings* Settings = nullptr;
	UGamedevHelperAssetNamingConvention* NamingConvention = nullptr;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>> ListView;
	TArray<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>> AssetList;
	TArray<FGamedevHelperRenamePreview> RenamePreviews;
	TEnumAsByte<EColumnSortMode::Type> CurrentSortMode = EColumnSortMode::Ascending;
	FName SortColumn = TEXT("AssetClass");
	bool bRenameBtnActive = false;
};