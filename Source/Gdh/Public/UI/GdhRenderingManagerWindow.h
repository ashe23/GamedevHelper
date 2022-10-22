// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhRenderingManagerListItem.h"
#include "GdhTypes.h"
#include "MoviePipelineMasterConfig.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhRenderingAssetsSettings;
class UMoviePipelineMasterConfig;
class UMoviePipelineQueue;
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

	void ListUpdate();
	void RegisterCommands();
	void ValidateSettings();
	void ValidateRenderingSettings();
	void ValidateRenderingAssetsSettings();
	FText GetJobStats() const;
	FText GetErrorMsgBoxText() const;
	int32 GetActiveWidgetIndex() const;
	TSharedPtr<SHeaderRow> GetHeaderRow() const;
	EVisibility GetErrorMsgBoxVisibility() const;
	TSharedPtr<SWidget> ListCreateContextMenu() const;
	void OnListItemDblClick(TWeakObjectPtr<UGdhRenderingManagerListItem> Item);
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGdhRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	UGdhRenderingManagerListItem* GetSelectedItemChecked() const;
	UMoviePipelineMasterConfig* CreateMasterConfig() const;
	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
	FIntPoint GetResolution() const;
	FString GetResolutionFolderName() const;
	FString GetImageExtension(const bool IncludeDot = false) const;
	FString GetVideoExtension(const bool IncludeDot = false) const;
	FString GetImageOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	FString GetVideoOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	FString GetVideoFilePath(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	FString GetVideoEncodeCmd(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	UClass* GetImageClass() const;
	bool MustRenderImages(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	bool MustEncodeVideo(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	void CalculateRenderedFrames(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	void RunFFmpegCommands();
	bool ContainsTimeDilationTrack(const ULevelSequence* LevelSequence) const;
	TWeakObjectPtr<UGdhRenderingManagerListItem> CreateListItem(const ULevelSequence* LevelSequence, const UWorld* Map, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;
	FReply OnBtnRefreshClick();
	FReply OnBtnRenderClick();
	bool IsBtnRenderEnabled() const;

	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhRenderingAssetsSettings* RenderingAssetsSettings = nullptr;
	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> ListItems;
	TArray<FGdhFFmpegCommand> FFmpegCommands;
	const int32 WidgetIndexWorking = 0;
	const int32 WidgetIndexIdle = 1;
	FString JobStats;
	FString ErrorMsg;
	int32 JobsTotal = 0;
	int32 JobsFinished = 0;
	double RenderStartTime = 0.0;
	bool bIsValidRenderingSettings = false;
	bool bIsValidRenderingAssetsSettings = false;
};


