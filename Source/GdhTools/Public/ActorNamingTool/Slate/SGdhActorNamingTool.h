// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhActorNamingToolListItem;
class UGdhActorNamingToolSettings;

class SGdhActorNamingTool final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhActorNamingTool) {}
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
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhActorNamingToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<FUICommandList> Cmds;
	TArray<TWeakObjectPtr<UGdhActorNamingToolListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhActorNamingToolListItem>>> ListView;
	TWeakObjectPtr<UGdhActorNamingToolSettings> ActorNamingToolSettings;
};
