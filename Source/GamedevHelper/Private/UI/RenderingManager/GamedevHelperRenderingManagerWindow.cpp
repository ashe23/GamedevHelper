// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerWindow.h"
#include "ProjectSettings/GamedevHelperRenderingSettings.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerListItem.h"
#include "UI/GamedevHelperEditorStyle.h"
// #include "GamedevHelperTypes.h"
// #include "GamedevHelperProjectSettings.h"
// Engine Headers
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MovieSceneTimeHelpers.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "AssetRegistryModule.h"
#include "GamedevHelperSubsystem.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineCommandLineEncoder.h"
#include "MoviePipelineCommandLineEncoderSettings.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineGameOverrideSetting.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineWidgetRenderSetting.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/ScopedSlowTask.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

void SGamedevHelperRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RenderingSettings = GetMutableDefault<UGamedevHelperRenderingSettings>();
	verify(RenderingSettings);

	RenderingSettings->OnSettingChanged().AddLambda([&](const UObject* Object, const FPropertyChangedEvent& PropertyChangedEvent)
	{
		ListUpdateData();
		ListRefresh();
	});

	RenderingManagerQueueSettings = GetMutableDefault<UGamedevHelperRenderingManagerQueueSettings>();
	verify(RenderingManagerQueueSettings);

	RenderingManagerQueueSettings->QueueSettingsDelegate.BindLambda([&]()
	{
		ListUpdateData();
		ListRefresh();
	});

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	AssetRegistryModule.Get().OnAssetUpdatedOnDisk().AddLambda([&](const FAssetData& AssetData)
	{
		// if changed asset is one of our queue assets then refresh data
		for (const auto& QueueAsset : RenderingManagerQueueSettings->QueueAssets)
		{
			if (QueueAsset.GetAssetPathString().Equals(AssetData.ObjectPath.ToString()))
			{
				ListUpdateData();
				ListRefresh();
				break;
			}
		}
	});

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GamedevHelperRenderingSettings";

	const TSharedPtr<IDetailsView> RenderingManagerSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingManagerSettingsProperty->SetObject(RenderingSettings);

	DetailsViewArgs.ViewIdentifier = "GamedevHelperRenderingManagerQueueSettings";
	
	const TSharedPtr<IDetailsView> RenderingManagerQueueProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingManagerQueueProperty->SetObject(RenderingManagerQueueSettings);

	ListUpdateData();
	ListRefresh();

	RenderingSettings->Validate();
	RenderingManagerQueueSettings->Validate();
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(FMargin{10.0f})
		.FillHeight(0.95f)
		[
			SNew(SSplitter)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			.PhysicalSplitterHandleSize(5.0f)
			+ SSplitter::Slot()
			.Value(0.4f)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(FMargin{10.0f})
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						RenderingManagerSettingsProperty.ToSharedRef()
					]
				]
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(FMargin{10.0f})
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						RenderingManagerQueueProperty.ToSharedRef()
					]
				]
			]
			+ SSplitter::Slot()
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(FMargin{10.0f})
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(FMargin{0.0f, 0.0f, 5.0f , 0.0f})
						.AutoWidth()
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
							.ContentPadding(FMargin{5})
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerWindow::OnBtnRefreshClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerWindow::IsBtnRefreshEnabled)
							.ToolTipText(FText::FromString(TEXT("Refreshes render queue list")))
							[
								SNew(STextBlock)
								.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Refresh")))
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(FMargin{0.0f, 0.0f, 5.0f , 0.0f})
						.AutoWidth()
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#66BB6A"))})
							.ContentPadding(FMargin{5})
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerWindow::OnBtnRenderClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerWindow::IsBtnRenderEnabled)
							.ToolTipText(FText::FromString(TEXT("Start rendering everything in list")))
							[
								SNew(STextBlock)
								.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Render")))
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(FMargin{0.0f, 0.0f, 5.0f , 0.0f})
						.AutoWidth()
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
							.ContentPadding(FMargin{5})
							.ToolTipText(FText::FromString(TEXT("Removes everything in OutputDirectory")))
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerWindow::OnBtnCleanOutputDirClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerWindow::IsBtnCleanOutputDirEnabled)
							[
								SNew(STextBlock)
								.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Clean Output Directory")))
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(FMargin{0.0f, 0.0f, 5.0f , 0.0f})
						.AutoWidth()
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#BDBDBD"))})
							.ContentPadding(FMargin{5})
							.ToolTipText(FText::FromString(TEXT("Opens OutputDirectory in Explorer")))
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerWindow::OnBtnOpenOutputDirClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerWindow::IsBtnOpenOutputDirEnabled)
							[
								SNew(STextBlock)
								.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Open Output Directory")))
							]
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SScrollBox)
						.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
						.AnimateWheelScrolling(true)
						.AllowOverscroll(EAllowOverscroll::No)
						+ SScrollBox::Slot()
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.Padding(FMargin{10.0f})
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.AutoHeight()
							[
								SAssignNew(ListView, SListView<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>>)
								.ListItemsSource(&ListItems)
								.SelectionMode(ESelectionMode::None)
								.OnGenerateRow(this, &SGamedevHelperRenderingManagerWindow::OnGenerateRow)
								.HeaderRow
								(
									SNew(SHeaderRow)
									+ SHeaderRow::Column(FName("State"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(32.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("StateColumn", "#"))
									]
									+ SHeaderRow::Column(FName("QueueName"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									// .FixedWidth(250.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("QueueNameColumn", "Queue"))
									]
									+ SHeaderRow::Column(FName("SequenceName"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									// .FixedWidth(250.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceNameColumn", "Level Sequence"))
									]
									+ SHeaderRow::Column(FName("SequenceDuration"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									// .FixedWidth(200.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceDurationColumn", "Duration"))
									]
									+ SHeaderRow::Column(FName("SequenceFrameStart"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(100.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceFrameStartColumn", "Start Frame"))
									]
									+ SHeaderRow::Column(FName("SequenceRenderedFrames"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(130.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceRenderedFramesColumn", "Rendered Frames"))
									]
									+ SHeaderRow::Column(FName("Note"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									[
										SNew(STextBlock)
										.Text(LOCTEXT("NoteColumn", "Note"))
									]
								)
							]
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		.Padding(FMargin{10.0f,5.0f})
		.FillHeight(0.05f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FGamedevHelperEditorStyle::GetIcon(TEXT("GamedevHelper.Icon.BG.16")))
			.BorderBackgroundColor(FLinearColor{FColor::FromHex(TEXT("#0277BD"))})
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Left)
					.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light15"))
					.Text_Raw(this, &SGamedevHelperRenderingManagerWindow::GetFFmpegCommandPreview)
				]
			]
		]
		+ SVerticalBox::Slot()
		.Padding(FMargin{10.0f,5.0f})
		.FillHeight(0.05f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.Visibility_Raw(this, &SGamedevHelperRenderingManagerWindow::GetConsoleBoxVisibility)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Left)
				.Text_Raw(this, &SGamedevHelperRenderingManagerWindow::GetConsoleBoxText)
				.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light10"))
				.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
			]
		]
	];
}

FText SGamedevHelperRenderingManagerWindow::GetConsoleBoxText() const
{
	if (!RenderingSettings->IsValid())
	{
		return FText::FromString(RenderingSettings->GetErrorMsg());
	}
	
	if (!RenderingManagerQueueSettings->IsValid())
	{
		return FText::FromString(RenderingManagerQueueSettings->GetErrorMsg());
	}
	
	return FText::FromString(TEXT(""));
}

EVisibility SGamedevHelperRenderingManagerWindow::GetConsoleBoxVisibility() const
{
	return RenderingSettings->IsValid() && RenderingManagerQueueSettings->IsValid() ? EVisibility::Hidden : EVisibility::Visible;
}

TSharedRef<ITableRow> SGamedevHelperRenderingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGamedevHelperRenderingManagerListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperRenderingManagerListItem, OwnerTable).QueueItem(InItem);
}

FText SGamedevHelperRenderingManagerWindow::GetFFmpegCommandPreview() const
{
	const FString Preview = FString::Printf(
		TEXT("%s -y -i {image_input} %s {output_video_file}"),
		*GetDefault<UMoviePipelineCommandLineEncoderSettings>()->ExecutablePath,
		*UKismetStringLibrary::JoinStringArray(RenderingSettings->FFmpegExtraCommands, TEXT(" "))
	);

	return FText::FromString(Preview);
}

void SGamedevHelperRenderingManagerWindow::ListUpdateData()
{
	if (!RenderingSettings || !RenderingManagerQueueSettings) return;
	
	ListItems.Reset();
	ListItems.Reserve(RenderingManagerQueueSettings->QueueAssets.Num());

	for (const auto& QueueAsset : RenderingManagerQueueSettings->QueueAssets)
	{
		const TSoftObjectPtr<UMoviePipelineQueue> QueueAssetData = QueueAsset.TryLoad();
		if (!QueueAssetData) continue;
		
		const TArray<UMoviePipelineExecutorJob*> QueueJobs = QueueAssetData->GetJobs();
		
		ListItems.Reserve(ListItems.Num() + QueueJobs.Num());
		
		for (const auto& Job : QueueJobs)
		{
			const TSoftObjectPtr<UGamedevHelperRenderingManagerListItem> NewListItem = NewObject<UGamedevHelperRenderingManagerListItem>();
			NewListItem->Status = EGamedevHelperRendererStatus::OK;
			NewListItem->QueueName = QueueAsset.GetAssetName();
			NewListItem->QueueAsset = QueueAsset;

			// errors
			if (!Job)
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Error;
				NewListItem->Note = TEXT("Contains Invalid Job");
				ListItems.Add(NewListItem.Get());
				continue;
			}

			const TSoftObjectPtr<ULevelSequence> LevelSequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			if (!LevelSequence)
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Error;
				NewListItem->Note = TEXT("Contains invalid LevelSequence");
				ListItems.Add(NewListItem.Get());
				continue;
			}

			if (!Job->Map.IsValid())
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Error;
				NewListItem->Note = TEXT("Map not specified");
				ListItems.Add(NewListItem.Get());
				continue;
			}

			NewListItem->LevelSequence = LevelSequence.Get();
			NewListItem->SequenceName = LevelSequence.GetAssetName();
			
			FGamedevHelperSequencePlaybackInfo PlaybackInfo;
			GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->GetLevelSequencePlaybackInfo(LevelSequence, PlaybackInfo);
			
			NewListItem->SequenceDuration = FString::Printf(TEXT("%d frames (%.2f seconds)"), PlaybackInfo.DurationInFrames, PlaybackInfo.DurationInSeconds);
			NewListItem->SequenceDurationInFrames = PlaybackInfo.DurationInFrames;
			NewListItem->SequenceStartFrame = PlaybackInfo.FrameStart;

			bool bIsSequential = false;
			NewListItem->SequenceRenderedFrames = GetRenderedFramesNum(QueueAssetData, LevelSequence,bIsSequential);

			// warnings
			if (!bIsSequential)
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Warning;
				NewListItem->Note = TEXT("Missing some rendered frames");
			}

			if (Job->JobName.IsEmpty())
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Warning;
				NewListItem->Note.Append(TEXT(" Job name is empty"));
			}

			ListItems.Add(NewListItem.Get());
		}
	}
}

void SGamedevHelperRenderingManagerWindow::ListRefresh() const
{
	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
}

bool SGamedevHelperRenderingManagerWindow::IsMovieRenderWorking() const
{
	if (!GEditor) return false;
	
	return GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering();
}

void SGamedevHelperRenderingManagerWindow::OnMovieRenderFinished(UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
{
	ListUpdateData();
	ListRefresh();
		
	if (!bSuccess)
	{
		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(TEXT("Error occured when rendering images"));
		return;
	}

	const float ElapsedTime = FPlatformTime::Seconds() - RenderStartTime;
	const FString Text = FString::Printf(TEXT("Rendering images finished successfully in %.1f seconds"), ElapsedTime);
	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModal(Text, EGamedevHelperModalStatus::Success);

	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->RunFFmpegPythonScript(FFmpegCommands);
}

void SGamedevHelperRenderingManagerWindow::OnMovieRenderError(UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
{
	ListUpdateData();
	ListRefresh();

	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(FString::Printf(TEXT("Renderer finished with errors: %s"),  *ErrorText.ToString()), 5.0f);
}

int32 SGamedevHelperRenderingManagerWindow::GetRenderedFramesNum(TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue, TSoftObjectPtr<ULevelSequence> LevelSequence, bool& IsSequential) const
{
	if (!MoviePipelineQueue || !LevelSequence) return 0;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	const FString Path = RenderingSettings->GetImageOutputDirectory(MoviePipelineQueue, LevelSequence);

	FGamedevHelperSequencePlaybackInfo PlaybackInfo;
	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->GetLevelSequencePlaybackInfo(LevelSequence, PlaybackInfo);

	struct DirectoryVisitor : IPlatformFile::FDirectoryVisitor
	{
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory) return false;
	
			const FString BaseName = FPaths::GetBaseFilename(FilenameOrDirectory);
			const FString Extension = FPaths::GetExtension(FilenameOrDirectory);
			if (BaseName.StartsWith(RequiredBaseName) && Extension.Equals(GetDefault<UGamedevHelperRenderingSettings>()->GetImageExtension()))
			{
				TArray<FString> Parts;
				BaseName.ParseIntoArray(Parts, TEXT("_"));
				Frames.Add(FCString::Atoi(*Parts.Last()));
			}
			
			return true;
		}
	
		FString RequiredBaseName;
		TArray<int32> Frames;
	};

	DirectoryVisitor Visitor;
	Visitor.RequiredBaseName = LevelSequence.GetAssetName();
	if (!PlatformFile.IterateDirectory(*Path, Visitor))
	{
		return 0;
	}

	if (Visitor.Frames.Num() == 0)
	{
		return 0;
	}

	Visitor.Frames.Sort();
	for (int32 i = 0; i < Visitor.Frames.Num(); ++i)
	{
		if (Visitor.Frames[i] != i)
		{
			IsSequential = false;
		}
	}

	IsSequential = true;

	return Visitor.Frames.Num();
}

FReply SGamedevHelperRenderingManagerWindow::OnBtnRefreshClicked()
{
	if (IsMovieRenderWorking()) return FReply::Handled();

	ListUpdateData();
	ListRefresh();

	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerWindow::OnBtnRenderClicked()
{
	if (IsMovieRenderWorking()) return FReply::Handled();
	if (!GEditor) return FReply::Handled();

	const TSoftObjectPtr<UMoviePipelineQueue> Queue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!Queue) return FReply::Handled();

	Queue->DeleteAllJobs();
	FFmpegCommands.Reset();
	
	for (const auto& QueueAsset : RenderingManagerQueueSettings->QueueAssets)
	{
		const TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue = QueueAsset.TryLoad();
		if (!MoviePipelineQueue) continue;
		
		for (const auto& QueueJob : MoviePipelineQueue->GetJobs())
		{
			TSoftObjectPtr<UMoviePipelineExecutorJob> Job = Queue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
			Job->SetConfiguration(RenderingSettings->GetMasterConfig());
			
			if (!Job) return FReply::Handled();

			Job->Sequence = QueueJob->Sequence;
			Job->Map = QueueJob->Map;
			Job->JobName = QueueJob->JobName;
			Job->Author = QueueJob->Author;
			Job->ShotInfo = QueueJob->ShotInfo;

			const TSoftObjectPtr<ULevelSequence> LevelSequence = Job->Sequence.TryLoad();
			if (!LevelSequence) continue;

			const TSoftObjectPtr<UMoviePipelineMasterConfig> Config = Job->GetConfiguration();
			if (!Config) continue;
			const TSoftObjectPtr<UMoviePipelineOutputSetting> OutputSetting = Cast<UMoviePipelineOutputSetting>(Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
			if (!OutputSetting) continue;

			FGamedevHelperSequencePlaybackInfo PlaybackInfo;
			GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->GetLevelSequencePlaybackInfo(LevelSequence, PlaybackInfo);
			
			OutputSetting->OutputDirectory.Path = RenderingSettings->GetImageOutputDirectory(MoviePipelineQueue, LevelSequence);
			OutputSetting->FileNameFormat = TEXT("{sequence_name}_{frame_number_rel}");
			OutputSetting->OutputResolution = RenderingSettings->GetResolution();
			OutputSetting->bUseCustomFrameRate = true;
			OutputSetting->OutputFrameRate = RenderingSettings->Framerate;
			OutputSetting->bOverrideExistingOutput = true;
			OutputSetting->ZeroPadFrameNumbers = 4;
			OutputSetting->FrameNumberOffset = 0;
			OutputSetting->HandleFrameCount = 0;
			OutputSetting->OutputFrameStep = 1;
			OutputSetting->bUseCustomPlaybackRange = true;
			OutputSetting->CustomStartFrame = PlaybackInfo.FrameStart;
			OutputSetting->CustomEndFrame = PlaybackInfo.FrameEnd;
			
			const FString VideoOutputDirectory = RenderingSettings->GetVideoOutputDirectory(MoviePipelineQueue);
			
			if (!FPaths::DirectoryExists(VideoOutputDirectory))
			{
				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				PlatformFile.CreateDirectoryTree(*VideoOutputDirectory);
			}

			const FString Cmd = FString::Printf(
				TEXT("%s -y -i %s/%s_%%04d.%s %s %s/%s.%s"),
				*GetDefault<UMoviePipelineCommandLineEncoderSettings>()->ExecutablePath,
				*RenderingSettings->GetImageOutputDirectory(MoviePipelineQueue, LevelSequence),
				*LevelSequence.GetAssetName(),
				*RenderingSettings->GetImageExtension(),
				*UKismetStringLibrary::JoinStringArray(RenderingSettings->FFmpegExtraCommands, TEXT(" ")),
				*VideoOutputDirectory,
				*LevelSequence.GetAssetName(),
				*RenderingSettings->GetVideoExtension()
			);
			FFmpegCommands.Add(FGamedevHelperFFmpegCommand{QueueAsset.GetAssetName(), LevelSequence.GetAssetName(), TEXT("NoAudio"), Cmd, TEXT("Encoding video")});
		}
	}

	RenderStartTime = FPlatformTime::Seconds();
	
	const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	if (!Executor) return FReply::Handled();
	
	Executor->OnExecutorFinished().AddRaw(this, &SGamedevHelperRenderingManagerWindow::OnMovieRenderFinished);
	Executor->OnExecutorErrored().AddRaw(this, &SGamedevHelperRenderingManagerWindow::OnMovieRenderError);
	
	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerWindow::OnBtnCleanOutputDirClicked()
{
	if (IsMovieRenderWorking()) return FReply::Handled();
	if (!GEditor) return FReply::Handled();
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	const FText DialogContent = FText::FromString(TEXT("Are you sure you want delete everything in OutputDirectory folder?"));
	const FText DialogTitle = FText::FromString(TEXT("Clean OutputDirectory"));
	
	const EAppReturnType::Type ReturnType = FMessageDialog::Open(
		EAppMsgType::YesNo,
		DialogContent,
		&DialogTitle
	);

	if (ReturnType == EAppReturnType::No || ReturnType == EAppReturnType::Cancel)
	{
		return FReply::Handled();
	}
	
	if (!PlatformFile.DeleteDirectoryRecursively(*RenderingSettings->OutputDirectory.Path))
	{
		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(TEXT("Failed to clean OutputDirectory"), 3.0f);
		return FReply::Handled();
	}

	PlatformFile.CreateDirectoryTree(*RenderingSettings->OutputDirectory.Path);

	ListUpdateData();
	ListRefresh();
	
	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModal(TEXT("OutputDirectory cleaned successfully"), EGamedevHelperModalStatus::Success, 3.0f);
	
	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerWindow::OnBtnOpenOutputDirClicked() const
{
	if (IsMovieRenderWorking()) return FReply::Handled();
	
	FPlatformProcess::ExploreFolder(*RenderingSettings->OutputDirectory.Path);
	
	return FReply::Handled();
}

bool SGamedevHelperRenderingManagerWindow::IsBtnRefreshEnabled() const
{
	return !IsMovieRenderWorking() && RenderingSettings->IsValid() && RenderingManagerQueueSettings->IsValid();
}

bool SGamedevHelperRenderingManagerWindow::IsBtnRenderEnabled() const
{
	return !IsMovieRenderWorking() && RenderingSettings->IsValid() && RenderingManagerQueueSettings->IsValid(); // todo:ashe23 maybe change this
}

bool SGamedevHelperRenderingManagerWindow::IsBtnCleanOutputDirEnabled() const
{
	return !IsMovieRenderWorking() && FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path);
}

bool SGamedevHelperRenderingManagerWindow::IsBtnOpenOutputDirEnabled() const
{
	return !IsMovieRenderWorking() && FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path);
}

#undef LOCTEXT_NAMESPACE
