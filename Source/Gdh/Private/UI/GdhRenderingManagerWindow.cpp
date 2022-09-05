// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "UI/GdhRenderingManagerListItem.h"
#include "Settings/GdhMovieRenderSettings.h"
#include "Settings/GdhRenderingSettings.h"
#include "Settings/GdhRenderingQueueSettings.h"
#include "GdhStyles.h"
#include "GdhSubsystem.h"
// Engine Headers
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MovieSceneTimeHelpers.h"
#include "Misc/ScopedSlowTask.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	MovieRenderSettings = GetMutableDefault<UGdhMovieRenderSettings>();
	QueueSettings = GetMutableDefault<UGdhRenderingQueueSettings>();
	GdhSubsystem = GEditor->GetEditorSubsystem<UGdhSubsystem>();

	check(RenderingSettings);
	check(MovieRenderSettings);
	check(QueueSettings);
	check(GdhSubsystem);

	RenderingSettings->OnSettingChanged().AddLambda([&](const UObject* Obj, const FPropertyChangedEvent& ChangedEvent)
	{
		ListUpdate();
	});

	QueueSettings->GdhQueueSettingsOnChangeDelegate.BindLambda([&]()
	{
		ListUpdate();
	});

	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhRenderingSettings";

	const TSharedPtr<IDetailsView> RenderingSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingSettingsProperty->SetObject(RenderingSettings);

	DetailsViewArgs.ViewIdentifier = "GdhMovieRenderSettings";
	const TSharedPtr<IDetailsView> MovieRenderSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	MovieRenderSettingsProperty->SetObject(MovieRenderSettings);

	DetailsViewArgs.ViewIdentifier = "GdhQueueSettings";
	const TSharedPtr<IDetailsView> QueueSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	QueueSettingsProperty->SetObject(QueueSettings);

	ListUpdate();

	ChildSlot
	[
		SNew(SWidgetSwitcher)
		.WidgetIndex_Raw(this, &SGdhRenderingManagerWindow::GetActiveWidgetIndex)
		+ SWidgetSwitcher::Slot()
		  .HAlign(HAlign_Fill)
		  .VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Center)
			[
				SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text(FText::FromString(TEXT("MovieRender currently working...")))
					.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light30"))
			]
		]
		+ SWidgetSwitcher::Slot()
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
				.Value(0.3f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					  .AutoHeight()
					  .Padding(FMargin{0.0f, 10.0f})
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Rendering Settings")))
						.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light20"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						RenderingSettingsProperty.ToSharedRef()
					]
					+ SVerticalBox::Slot()
					  .AutoHeight()
					  .Padding(FMargin{0.0f, 10.0f})
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("MovieRender Settings")))
						.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light20"))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						MovieRenderSettingsProperty.ToSharedRef()
					]
				]
				+ SSplitter::Slot()
				.Value(0.3f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					  .AutoHeight()
					  .Padding(FMargin{5.0f, 10.0f})
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Queue Settings")))
						.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light20"))
					]
					+ SVerticalBox::Slot()
					  .Padding(FMargin{5.0f, 0.0f})
					  .AutoHeight()
					[
						QueueSettingsProperty.ToSharedRef()
					]
				]
				+ SSplitter::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					  .Padding(FMargin{10.0f})
					  .AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						  .Padding(FMargin{0.0f, 0.0f, 5.0f, 0.0f})
						  .AutoWidth()
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							// .ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
							.ContentPadding(FMargin{5})
							.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRefreshClick)
							// .IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRefreshEnabled)
							.ToolTipText(FText::FromString(TEXT("Refresh list")))
							[
								SNew(STextBlock)
								.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Refresh")))
							]
						]
						+ SHorizontalBox::Slot()
						  .Padding(FMargin{0.0f, 0.0f, 5.0f, 0.0f})
						  .AutoWidth()
						[
							SNew(SButton)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#FF5722"))})
							.ContentPadding(FMargin{5})
							.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRenderClick)
							// .IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRefreshEnabled)
							.ToolTipText(FText::FromString(TEXT("Render everything in list")))
							[
								SNew(STextBlock)
								.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Render")))
							]
						]
					]
					+ SVerticalBox::Slot()
					  .Padding(FMargin{10.0f})
					  .AutoHeight()
					[
						SNew(STextBlock)
						.Text_Raw(this, &SGdhRenderingManagerWindow::GetJobStats)
						.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light8"))
					]
					+ SVerticalBox::Slot()
					  .Padding(FMargin{10.0f})
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Fill)
					  .AutoHeight()
					[
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>)
						.ListItemsSource(&ListItems)
						.SelectionMode(ESelectionMode::SingleToggle)
						// .OnContextMenuOpening(this, &SGamedevHelperRenderingManagerWindow::ListCreateContextMenu)
						.OnGenerateRow(this, &SGdhRenderingManagerWindow::OnGenerateRow)
						// .OnMouseButtonDoubleClick(this, &SGamedevHelperRenderingManagerWindow::OnListDblClick)
						.HeaderRow(GetHeaderRow())
					]
				]
			]
			+ SVerticalBox::Slot()
			  .Padding(FMargin{10.0f})
			  .FillHeight(0.05f)
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
					.Visibility_Raw(this, &SGdhRenderingManagerWindow::GetConsoleBoxVisibility)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Left)
						.Text_Raw(this, &SGdhRenderingManagerWindow::GetConsoleBoxText)
						.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light15"))
						.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
					]
				]
			]
		]
	];
}

int32 SGdhRenderingManagerWindow::GetActiveWidgetIndex() const
{
	if (!GEditor) return WidgetIndexRenderingManagerWorking;

	if (GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering()) return WidgetIndexRenderingManagerWorking;

	return WidgetIndexRenderingManagerUI;
}

FText SGdhRenderingManagerWindow::GetConsoleBoxText() const
{
	if (RenderingSettings->OutputDirectory.Path.IsEmpty())
	{
		return FText::FromString(TEXT("Output directory not specified"));
	}

	if (!FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path))
	{
		return FText::FromString(TEXT("Output directory does not exist"));
	}

	if (RenderingSettings->FFmpegExe.FilePath.IsEmpty())
	{
		return FText::FromString(TEXT("FFmpeg.exe path not specified. Must be absolute path to exe, or just can be ffmpeg.exe if you have it in system PATHS"));
	}

	if (!RenderingSettings->FFmpegExe.FilePath.ToLower().Equals(TEXT("ffmpeg.exe")) && !FPaths::FileExists(RenderingSettings->FFmpegExe.FilePath))
	{
		return FText::FromString(FString::Printf(TEXT("Cant find ffmpeg.exe at given '%s' location"), *RenderingSettings->FFmpegExe.FilePath));
	}

	if (RenderingSettings->GetResolution().X % 2 != 0 || RenderingSettings->GetResolution().Y % 2 != 0)
	{
		return FText::FromString(TEXT("Resolution must have dimensions divisible by 2"));
	}

	const UMoviePipelineMasterConfig* MasterConfig = MovieRenderSettings->CreateMasterConfig();
	if (!GdhSubsystem->IsValidMasterConfig(MasterConfig))
	{
		return FText::FromString(GdhSubsystem->GetMasterConfigValidationMsg(MasterConfig));
	}

	if (!ErrorMsg.IsEmpty())
	{
		return FText::FromString(ErrorMsg);
	}

	return FText::FromString(TEXT(""));
}

FText SGdhRenderingManagerWindow::GetJobStats() const
{
	return FText::FromString(JobStats);
}

EVisibility SGdhRenderingManagerWindow::GetConsoleBoxVisibility() const
{
	return RenderingSettings->IsValidSettings() && GdhSubsystem->IsValidMasterConfig(MovieRenderSettings->CreateMasterConfig()) && ErrorMsg.IsEmpty() ? EVisibility::Hidden : EVisibility::Visible;
}

TSharedRef<ITableRow> SGdhRenderingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGdhRenderingManagerListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGdhRenderingManagerListItem, OwnerTable).ListItem(InItem);
}

TSharedPtr<SHeaderRow> SGdhRenderingManagerWindow::GetHeaderRow() const
{
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(FName("Status"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FixedWidth(32.0f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Job Status")))
			.Text(FText::FromString(TEXT("#")))
		]
		+ SHeaderRow::Column(FName("Sequence"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillSized(200.0f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("LevelSequence Name")))
			.Text(FText::FromString(TEXT("Sequence")))
		]
		+ SHeaderRow::Column(FName("Duration"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillSized(200.0f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("LevelSequence duration, based on framerate parameter. Does not take into account time dilation track")))
			.Text(FText::FromString(TEXT("Duration")))
		]
		+ SHeaderRow::Column(FName("RenderedFrames"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Number of already rendered frames")))
			.Text(FText::FromString(TEXT("Rendered Frames Num")))
		]
		// + SHeaderRow::Column(FName("RenderedFramesDir"))
		//   .HAlignCell(HAlign_Center)
		//   .VAlignCell(VAlign_Center)
		//   .HAlignHeader(HAlign_Center)
		//   .HeaderContentPadding(FMargin(10.0f))
		//   .FillSized(500.0f)
		// [
		// 	SNew(STextBlock)
		// 	.ToolTipText(FText::FromString(TEXT("Job rendered frames image directory")))
		// 	.Text(FText::FromString(TEXT("ImageDir")))
		// ]
		// + SHeaderRow::Column(FName("RenderedVideoFile"))
		//   .HAlignCell(HAlign_Center)
		//   .VAlignCell(VAlign_Center)
		//   .HAlignHeader(HAlign_Center)
		//   .HeaderContentPadding(FMargin(10.0f))
		//   .FillSized(500.0f)
		// [
		// 	SNew(STextBlock)
		// 	.ToolTipText(FText::FromString(TEXT("Job encoded video file")))
		// 	.Text(FText::FromString(TEXT("VideoFile")))
		// ]
		+ SHeaderRow::Column(FName("Note"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillSized(500.0f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Job encoded video file")))
			.Text(FText::FromString(TEXT("Note")))
		];
}

void SGdhRenderingManagerWindow::ListUpdate()
{
	if (!GEditor) return;

	ErrorMsg.Reset();
	JobStats.Reset();

	if (QueueSettings->LevelSequences.Num() == 0)
	{
		ErrorMsg = TEXT("Queue is empty. Select some LevelSequences in order to render");
		return;
	}

	ListItems.Reset();
	ListItems.Reserve(QueueSettings->LevelSequences.Num());

	FScopedSlowTask LevelSequenceSlowTask{
		static_cast<float>(QueueSettings->LevelSequences.Num()),
		FText::FromString(TEXT("Loading Queue assets..."))
	};
	LevelSequenceSlowTask.MakeDialog();

	double TotalJobsDuration = 0;

	for (const auto& LevelSequenceSetting : QueueSettings->LevelSequences)
	{
		LevelSequenceSlowTask.EnterProgressFrame(1.0f);
		if (LevelSequenceSlowTask.ShouldCancel())
		{
			break;
		}

		ULevelSequence* LevelSequence = LevelSequenceSetting.LevelSequence.LoadSynchronous();
		if (!LevelSequence)
		{
			ErrorMsg = TEXT("Failed to load some of LevelSequence assets in Queue");
			return;
		}


		UWorld* Map = LevelSequenceSetting.bUseEditorMap ? GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld() : LevelSequenceSetting.Map.LoadSynchronous();
		if (!Map)
		{
			ErrorMsg = TEXT("Failed to load some Map assets in queue");
			return;
		}

		UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
		if (!ListItem) continue;

		ListItem->LevelSequence = LevelSequence;
		ListItem->Map = Map;

		TotalJobsDuration += GdhSubsystem->GetLevelSequenceDuration(LevelSequence) / RenderingSettings->Framerate.AsDecimal();

		ListItems.Add(ListItem);
	}

	if (ListView.IsValid())
	{
		ListView->RequestListRefresh();
	}

	JobStats = FString::Printf(TEXT("%d %s (total duration %.1f seconds)"), ListItems.Num(), ListItems.Num() > 1 ? TEXT("jobs") : TEXT("job"), TotalJobsDuration);
}

FReply SGdhRenderingManagerWindow::OnBtnRefreshClick()
{
	ListUpdate();

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClick()
{
	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue)
	{
		GdhSubsystem->ShowModalWithOutputLog(TEXT("Failed to create queue for rendering"), EGdhModalStatus::Error, 5.0f);
		return FReply::Handled();
	}

	CustomQueue->DeleteAllJobs();

	for (const auto& ListItem : ListItems)
	{
		if (!ListItem.IsValid()) continue;

		UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
		if (!Job)
		{
			UE_LOG(LogGdh, Error, TEXT("Failed to create job for custom queue"));
			return FReply::Handled();
		}
		Job->Sequence = ListItem->LevelSequence;
		Job->Map = ListItem->Map;
		Job->JobName = ListItem->LevelSequence->GetName();
		Job->SetConfiguration(MovieRenderSettings->CreateMasterConfig());

		UMoviePipelineMasterConfig* Config = Job->GetConfiguration();
		if (!Config)
		{
			return FReply::Handled();
		}

		Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
		Config->FindOrAddSettingByClass(RenderingSettings->GetImageClass());

		UMoviePipelineOutputSetting* OutputSetting = Cast<UMoviePipelineOutputSetting>(Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
		if (!OutputSetting)
		{
			return FReply::Handled();
		}

		const int32 FrameStart = ConvertFrameTime(
			UE::MovieScene::DiscreteInclusiveLower(ListItem->LevelSequence->MovieScene->GetPlaybackRange()),
			ListItem->LevelSequence->MovieScene->GetTickResolution(),
			ListItem->LevelSequence->MovieScene->GetDisplayRate()
		).FloorToFrame().Value;
		const int32 FrameEnd = ConvertFrameTime(
			UE::MovieScene::DiscreteExclusiveUpper(ListItem->LevelSequence->MovieScene->GetPlaybackRange()),
			ListItem->LevelSequence->MovieScene->GetTickResolution(),
			ListItem->LevelSequence->MovieScene->GetDisplayRate()
		).FloorToFrame().Value;

		OutputSetting->OutputDirectory.Path = GdhSubsystem->GetImageOutputDirectoryPath(ListItem->LevelSequence);
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
		OutputSetting->CustomStartFrame = FrameStart;
		OutputSetting->CustomEndFrame = FrameEnd;
	}

	const FTimespan RenderStartTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
	
	const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	if (!Executor) return FReply::Handled();

	Executor->OnExecutorFinished().AddLambda([&](UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
	{
		const FTimespan RenderEndTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
		
		ListUpdate();

		if (!GdhSubsystem) return;

		if (!bSuccess)
		{
			GdhSubsystem->ShowModalWithOutputLog(TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
			return;
		}

		
		const FString SuccessText = FString::Printf(TEXT("Images rendered successfully in %d sec"), (RenderEndTime - RenderStartTime).GetSeconds());
		UE_LOG(LogGdh, Log, TEXT("%s"), *SuccessText);
		GdhSubsystem->ShowModal(SuccessText, EGdhModalStatus::OK, 5.0f);
	});
	Executor->OnExecutorErrored().AddLambda([&](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
	{
		ListUpdate();

		if (!GdhSubsystem) return;

		GdhSubsystem->ShowModalWithOutputLog(TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
		UE_LOG(LogGdh, Error, TEXT("%s"), *ErrorText.ToString());
	});

	return FReply::Handled();
}

bool SGdhRenderingManagerWindow::IsBtnRenderEnabled() const
{
	if (ListItems.Num() == 0) return false;

	for (const auto& ListItem : ListItems)
	{
		if (!ListItem.IsValid() || !ListItem->LevelSequence || !ListItem->Map)
		{
			return false;
		}
	}

	return true;
}
