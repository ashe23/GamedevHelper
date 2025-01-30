// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UMoviePipelineExecutorBase;
class UGdhVideoEncoderToolSettings;
class UGdhVideoEncoderToolListItem;

class SGdhVideoEncoderTool final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhVideoEncoderTool) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	void ListUpdateData();
	void ListUpdateView();
	void CmdsRegister();
	void OnRefreshPipelines();	 // TODO:ashe23 rename later
	void OnProcess();
	void OnRenderFinished(UMoviePipelineExecutorBase*, bool);
	TSharedRef<SWidget> CreateToolbarMain() const;
	TSharedRef<SHeaderRow> GetHeaderRow();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhVideoEncoderToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<FUICommandList> Cmds;
	TArray<TWeakObjectPtr<UGdhVideoEncoderToolListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhVideoEncoderToolListItem>>> ListView;
	TWeakObjectPtr<UGdhVideoEncoderToolSettings> VideoEncoderToolSettings;

	// FProcHandle ProcessHandle;
	TArray<FString> EncodeCmds;
};
