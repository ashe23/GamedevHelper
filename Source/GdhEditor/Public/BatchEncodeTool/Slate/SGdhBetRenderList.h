// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhBetRenderList;
class UGdhBetRenderListItem;
class UMoviePipelineExecutorBase;

class SGdhBetRenderList final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhBetRenderList) {}
	SLATE_ARGUMENT(UGdhBetRenderList*, RenderList)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	void ListUpdate();
	void ListUpdateData();
	void ListUpdateView() const;
	void OnListRefresh();
	void OnListRemove();
	void OnListRemoveAll();
	void OnRencode();
	void OnRencodeFinished(UMoviePipelineExecutorBase*, bool);
	bool CanRemoveListItems();
	void OnListDblClick(TWeakObjectPtr<UGdhBetRenderListItem> Item);

	FReply OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);
	bool CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);
	FText GetSummaryTxt() const;
	TSharedRef<SWidget> CreateToolbarMain() const;
	static TSharedRef<SHeaderRow> GetHeaderRow();
	static TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhBetRenderListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TArray<FString> EncodeCmds;
	TArray<TWeakObjectPtr<UGdhBetRenderListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhBetRenderListItem>>> ListView;
	TSharedPtr<FUICommandList> Cmds;
	UGdhBetRenderList* RenderList = nullptr;
};
