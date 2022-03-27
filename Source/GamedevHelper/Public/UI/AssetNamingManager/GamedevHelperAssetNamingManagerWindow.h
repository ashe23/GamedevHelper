// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGamedevHelperAssetNamingManagerSettings;
class UGamedevHelperAssetNamingConventionSettings;
class UGamedevHelperProjectSettings;
class UGamedevHelperAssetNamingListItem;
struct FGamedevHelperRenamePreview;

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
	static void OnListItemDblClick(TWeakObjectPtr<UGamedevHelperAssetNamingListItem> Item);
	void OnRenameSelected();
	void OnOpenSelected() const;
	FReply OnRenameBtnClick();
	FReply OnRefreshBtnClick();
	FReply OnClearSelectionBtnClick() const;
	bool IsRenameBtnEnabled() const;
	bool IsClearSelectionBtnEnabled() const;

	TSharedPtr<FUICommandList> PluginCommands;
	UGamedevHelperAssetNamingConventionSettings* NamingConventionSettings = nullptr;
	UGamedevHelperAssetNamingManagerSettings* Settings = nullptr;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>> ListView;
	TArray<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>> AssetList;
	TArray<FGamedevHelperRenamePreview> RenamePreviews;
	TEnumAsByte<EColumnSortMode::Type> CurrentSortMode = EColumnSortMode::Ascending;
	FName SortColumn = TEXT("AssetClass");
	bool bRenameBtnActive = false;
};