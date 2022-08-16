// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "Widgets/SCompoundWidget.h"

class ULevelSequence;
class UMoviePipelineQueue;
class UMoviePipeline;
class UMoviePipelineExecutorBase;
class UGamedevHelperRenderingSettings;
class UGamedevHelperRenderingManagerQueueSettings;
class UGamedevHelperRenderingManagerListItem;

class SGamedevHelperRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRenderingManagerWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
protected:
	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGamedevHelperRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	FText GetFFmpegCommandPreview() const;

	void ListUpdateData();
	void ListRefresh() const;
	bool IsMovieRenderWorking() const;
	void OnMovieRenderFinished(UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess);
	void OnMovieRenderError(UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText);
	int32 GetRenderedFramesNum(TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue, TSoftObjectPtr<ULevelSequence> LevelSequence, bool& IsSequential) const;

	FReply OnBtnRefreshClicked();
	FReply OnBtnRenderClicked();
	FReply OnBtnCleanOutputDirClicked();
	FReply OnBtnOpenOutputDirClicked() const;
	bool IsBtnRefreshEnabled() const;
	bool IsBtnRenderEnabled() const;
	bool IsBtnCleanOutputDirEnabled() const;
	bool IsBtnOpenOutputDirEnabled() const;
	
private:
	UGamedevHelperRenderingSettings* RenderingSettings = nullptr;
	UGamedevHelperRenderingManagerQueueSettings* RenderingManagerQueueSettings = nullptr;
	float RenderStartTime = 0.0f;

	TArray<FGamedevHelperFFmpegCommand> FFmpegCommands;
	TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>>> ListView;
};


