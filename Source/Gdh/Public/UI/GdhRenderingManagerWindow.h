// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhMovieRenderSettings;
class UGdhRenderingQueueSettings;
class UGdhRenderingManagerListItem;

class SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	int32 GetActiveWidgetIndex() const;
	FText GetConsoleBoxText() const;
	FText GetJobStats() const;
	EVisibility GetConsoleBoxVisibility() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGdhRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	TSharedPtr<SHeaderRow> GetHeaderRow() const;
	TSharedPtr<SWidget> ListCreateContextMenu() const;
	void RegisterCommands();

	void ListUpdate();
	void ValidateSettings();

	FReply OnBtnRefreshClick();
	FReply OnBtnRenderClick();

	bool IsBtnRenderEnabled() const;
	bool IsBtnRefreshEnabled() const;
	
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhMovieRenderSettings* MovieRenderSettings = nullptr;
	UGdhRenderingQueueSettings* QueueSettings = nullptr;
	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> ListItems;
	TArray<FGdhFFmpegCommand> FFmpegCommands;
	const int32 WidgetIndexRenderingManagerWorking = 0;
	const int32 WidgetIndexRenderingManagerUI = 1;
	FString JobStats;
	FString ConsoleBoxText;
	bool bIsValidSettings = false;
	bool bCanStartRendering = false;
};