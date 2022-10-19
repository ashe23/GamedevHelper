// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "MoviePipelineMasterConfig.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhRenderingAssetsSettings;
class UMoviePipelineMasterConfig;
class UMoviePipelineQueue;
// class UGdhMovieRenderSettings;
// class UGdhRenderingQueueSettings;
class UGdhRenderingManagerListItem;

class SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	int32 GetActiveWidgetIndex() const;
	FText GetErrorMsgBoxText() const;
	EVisibility GetErrorMsgBoxVisibility() const;
	FText GetJobStats() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGdhRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	TSharedPtr<SHeaderRow> GetHeaderRow() const;
	TSharedPtr<SWidget> ListCreateContextMenu() const;
	void RegisterCommands();

	void ListUpdate();
	void ValidateSettings();
	void ValidateRenderingSettings();
	void ValidateRenderingAssetsSettings();
	UMoviePipelineMasterConfig* CreateMasterConfig() const;
	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
	FIntPoint GetResolution() const;
	FString GetResolutionFolderName() const;
	FString GetImageExtension(const bool IncludeDot = false) const;
	FString GetVideoExtension(const bool IncludeDot = false) const;
	FString GetImageOutputDir(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue) const;
	int32 GetRenderedFramesNum(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue, bool& bHasMissingFrames) const;
	bool ContainsTimeDilationTrack(const ULevelSequence* LevelSequence) const;
	TWeakObjectPtr<UGdhRenderingManagerListItem> CreateListItem(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;
	FReply OnBtnRefreshClick();
	FReply OnBtnRenderClick();
	bool IsBtnRenderEnabled() const;
	bool IsBtnRefreshEnabled() const;

	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhRenderingAssetsSettings* RenderingAssetsSettings = nullptr;
	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> ListItems;
	TArray<FGdhFFmpegCommand> FFmpegCommands;
	const int32 WidgetIndexRenderingManagerWorking = 0;
	const int32 WidgetIndexRenderingManagerUI = 1;
	FString JobStats;
	FString ErrorMsg;

	bool bIsValidRenderingSettings = false;
	bool bIsValidRenderingAssetsSettings = false;
};
