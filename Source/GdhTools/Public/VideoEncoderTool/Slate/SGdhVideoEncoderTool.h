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
	void ListUpdateView() const;
	void CmdsRegister();
	void OnVetRefresh();
	void OnVetProcess();
	void OnRenderFinished(UMoviePipelineExecutorBase*, bool);
	void ValidateSettings();
	TSharedRef<SWidget> CreateToolbarMain() const;
	TSharedRef<SHeaderRow> GetHeaderRow();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhVideoEncoderToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<FUICommandList> Cmds;
	TArray<TWeakObjectPtr<UGdhVideoEncoderToolListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhVideoEncoderToolListItem>>> ListView;
	TWeakObjectPtr<UGdhVideoEncoderToolSettings> VideoEncoderToolSettings;

	FString FFmpegPath;
	TArray<FString> EncodeCmds;
};
