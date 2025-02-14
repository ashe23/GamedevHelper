// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UMoviePipelineExecutorBase;
class UGdhBetSettings;
class UGdhBetQueueItem;

class SGdhBet final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhBet) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	void OnQueueRefresh();
	void OnQueueProcess();
	void QueueUpdateData();
	void QueueUpdateView();
	void OnRenderFinished(UMoviePipelineExecutorBase* Executor, bool bSuccess);

	static void OnRenderListDblClick(const FAssetData& AssetData);
	static void OnEncodePresetDblClick(const FAssetData& AssetData);
	FReply OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);
	bool CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);
	FText GetQueueSummaryTxt() const;

	TSharedRef<SHeaderRow> GetQueueHeaderRow();
	TSharedRef<ITableRow> OnQueueGenerateRow(TWeakObjectPtr<UGdhBetQueueItem> Item, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<SWidget> CreateToolbarQueue() const;
	TSharedPtr<FUICommandList> Cmds;
	UGdhBetSettings* Settings = nullptr;
	TArray<TWeakObjectPtr<UGdhBetQueueItem>> QueueItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhBetQueueItem>>> QueueView;

	TArray<FString> EncodeCmds;
};
