// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhAntListItem;
class UGdhAntSettings;

class SGdhAnt final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhAnt) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	void CmdsRegister();
	void OnScanActors();
	void OnOrganizeActors();
	void OnRenameActors();
	void OnUndo();

	void UpdateListData();
	void UpdateListView() const;

	TSharedRef<SWidget> CreateToolbarMain() const;
	TSharedRef<SHeaderRow> GetHeaderRow();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhAntListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<FUICommandList> Cmds;
	TArray<TWeakObjectPtr<UGdhAntListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhAntListItem>>> ListView;
	TWeakObjectPtr<UGdhAntSettings> AntSettings;
};
