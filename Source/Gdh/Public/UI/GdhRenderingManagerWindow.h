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
class UGdhRenderingManagerListItem;

class SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	void RegisterCommands();
	FText GetJobStats() const;
	FText GetErrorMsgBoxText() const;
	EVisibility GetErrorMsgBoxVisibility() const;
	int32 GetActiveWidgetIndex() const;
	TSharedPtr<SHeaderRow> GetHeaderRow() const;
	TSharedPtr<SWidget> ListCreateContextMenu() const;
	void OnListItemDblClick(TWeakObjectPtr<UGdhRenderingManagerListItem> Item);
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhRenderingManagerListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const;
	FReply OnBtnRefreshClick();
	FReply OnBtnRenderClick();
	FReply OnBtnOutputDirCleanClick();
	FReply OnBtnOutputDirOpenClick();
	bool IsBtnRenderEnabled() const;
	bool IsBtnOutputDirCleanEnabled() const;
	bool IsBtnOutputDirOpenEnabled() const;

	void ListUpdate();
	void ValidateSettings();
	void ValidateRenderingSettings();
	void ValidateRenderingAssetsSettings();
	UGdhRenderingManagerListItem* GetSelectedItemChecked() const;
	UMoviePipelineMasterConfig* CreateMasterConfig() const;
	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
	UClass* GetImageClass() const;
	FIntPoint GetResolution() const;
	FString GetResolutionFolderName() const;
	FString GetImageExtension(const bool IncludeDot = false) const;
	FString GetVideoExtension(const bool IncludeDot = false) const;
	FString GetImageOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	FString GetVideoOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	FString GetVideoFilePath(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	FString GetVideoEncodeCmd(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	bool MustRenderImages(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	bool MustEncodeVideo(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	bool ContainsTimeDilationTrack(const ULevelSequence* LevelSequence) const;
	void CalculateRenderedFrames(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const;
	void ImagesRender(const TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>>& Items, const bool bForceRender = false, const bool bEncodeAfterRender = false);
	void RunFFmpegCommands();
	TWeakObjectPtr<UGdhRenderingManagerListItem> CreateListItem(const ULevelSequence* LevelSequence, const UWorld* Map, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;

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
