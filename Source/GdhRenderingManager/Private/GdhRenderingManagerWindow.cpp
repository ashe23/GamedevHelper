// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManagerWindow.h"

#include "GdhCoreStyles.h"
#include "GdhRenderingSettings.h"
#include "GdhRenderingQueueSettings.h"
#include "GdhRenderingManagerWindowListItem.h"
// Engine Headers
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Views/SListView.h"

void SGdhRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	RenderingQueueSettings = GetMutableDefault<UGdhRenderingQueueSettings>();

	check(RenderingSettings);
	check(RenderingQueueSettings);

	RenderingSettings->Validate();
	RenderingQueueSettings->Validate();

	RenderingSettings->OnSettingChanged().AddLambda([&](const UObject* Obj, const FPropertyChangedEvent& ChangedEvent)
	{
		ListUpdate();
	});

	RenderingQueueSettings->QueueSettingsChanged.BindLambda([&]()
	{
		ListUpdate();
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
	DetailsViewArgs.ViewIdentifier = "GdhRenderingSettings";

	const TSharedPtr<IDetailsView> RenderingSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingSettingsProperty->SetObject(RenderingSettings);

	DetailsViewArgs.ViewIdentifier = "GdhRenderingQueueSettings";
	const TSharedPtr<IDetailsView> RenderingQueueSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingQueueSettingsProperty->SetObject(RenderingQueueSettings);

	ListUpdate();

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
			.Value(0.3f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					SNew(STextBlock)
					.Font(FGdhCoreStyles::Get().GetFontStyle("GamedevHelper.Font.Light15"))
					.Text(FText::FromString(TEXT("Rendering Settings")))
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					RenderingSettingsProperty.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					SNew(STextBlock)
					.Font(FGdhCoreStyles::Get().GetFontStyle("GamedevHelper.Font.Light15"))
					.Text(FText::FromString(TEXT("Queue Settings")))
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					RenderingQueueSettingsProperty.ToSharedRef()
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
					SAssignNew(ListJobWidgetSwitcher, SWidgetSwitcher)
					.WidgetIndex(0)
					+ SWidgetSwitcher::Slot()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						  .FillHeight(1.0f)
						  .HAlign(HAlign_Center)
						  .VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("No Jobs")))
							.Justification(ETextJustify::Center)
							.Font(FGdhCoreStyles::Get().GetFontStyle("GamedevHelper.Font.Light30"))
						]
					]
					+ SWidgetSwitcher::Slot()
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
								.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
								.ContentPadding(FMargin{5})
								// .OnClicked_Raw(this, &SGamedevHelperRenderingManagerWindow::OnBtnRefreshClicked)
								// .IsEnabled_Raw(this, &SGamedevHelperRenderingManagerWindow::IsBtnRefreshEnabled)
								.ToolTipText(FText::FromString(TEXT("Refresh list")))
								[
									SNew(STextBlock)
									.Font(FGdhCoreStyles::Get().GetFontStyle("GamedevHelper.Font.Light10"))
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
								.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRenderClicked)
								// .IsEnabled_Raw(this, &SGamedevHelperRenderingManagerWindow::IsBtnRefreshEnabled)
								.ToolTipText(FText::FromString(TEXT("Render everything in list")))
								[
									SNew(STextBlock)
									.Font(FGdhCoreStyles::Get().GetFontStyle("GamedevHelper.Font.Light10"))
									.Text(FText::FromString(TEXT("Render")))
								]
							]
						]
						+ SVerticalBox::Slot()
						  .Padding(FMargin{10.0f})
						  .HAlign(HAlign_Fill)
						  .VAlign(VAlign_Fill)
						  .AutoHeight()
						[
							SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>)
							.ListItemsSource(&ListItems)
							.SelectionMode(ESelectionMode::None)
							// .OnContextMenuOpening(this, &SGamedevHelperRenderingManagerWindow::ListCreateContextMenu)
							.OnGenerateRow(this, &SGdhRenderingManagerWindow::OnGenerateRow)
							// .OnMouseButtonDoubleClick(this, &SGamedevHelperRenderingManagerWindow::OnListDblClick)
							.HeaderRow(GetHeaderRow())
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		  .Padding(FMargin{10.0f, 5.0f})
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
					.Font(FGdhCoreStyles::Get().GetFontStyle("GamedevHelper.Font.Light15"))
					.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
				]
			]
		]
	];
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
		+ SHeaderRow::Column(FName("RenderedFrames"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Number of already rendered frames")))
			.Text(FText::FromString(TEXT("RenderedFramesNum")))
		]
		+ SHeaderRow::Column(FName("RenderedFramesDir"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillSized(500.0f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Job rendered frames image directory")))
			.Text(FText::FromString(TEXT("ImageDir")))
		]
		+ SHeaderRow::Column(FName("RenderedVideoFile"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillSized(500.0f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Job encoded video file")))
			.Text(FText::FromString(TEXT("VideoFile")))
		]
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

FText SGdhRenderingManagerWindow::GetConsoleBoxText() const
{
	if (!RenderingSettings->IsValid())
	{
		return FText::FromString(RenderingSettings->GetErrorMsg());
	}

	if (!RenderingQueueSettings->IsValid())
	{
		return FText::FromString(RenderingQueueSettings->GetErrorMsg());
	}

	return FText::FromString(TEXT(""));
}

EVisibility SGdhRenderingManagerWindow::GetConsoleBoxVisibility() const
{
	return RenderingSettings->IsValid() && RenderingQueueSettings->IsValid() ? EVisibility::Hidden : EVisibility::Visible;
}

bool SGdhRenderingManagerWindow::IsMovieRenderWorking() const
{
	if (!GEditor) return false;
	
	return GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClicked()
{
	if (IsMovieRenderWorking()) return FReply::Handled();

	RenderingSettings->Validate();
	RenderingQueueSettings->Validate();

	if (!RenderingSettings->IsValid() || !RenderingQueueSettings->IsValid()) return FReply::Handled();

	if (!GEditor) return FReply::Handled();
	
	const UWorld* MapAsset = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
	if (!MapAsset) return FReply::Handled();

	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue) FReply::Handled();

	CustomQueue->DeleteAllJobs();

	for (const auto& LevelSequence : RenderingQueueSettings->LevelSequences)
	{
		UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
		if (!Job) return FReply::Handled();

		Job->SetConfiguration(RenderingSettings->GetMasterConfig(LevelSequence.Get()));
		Job->Map = MapAsset;
		Job->JobName = LevelSequence->GetName();
		Job->SetSequence(LevelSequence.Get());
	}

	for (const auto& MoviePipelineQueue : RenderingQueueSettings->MoviePipelineQueues)
	{
		for (const auto& QueueJob : MoviePipelineQueue->GetJobs())
		{
			UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
			if (!Job) return FReply::Handled();
			
			const ULevelSequence* Sequence = Cast<ULevelSequence>(QueueJob->Sequence.TryLoad());
			if (!Sequence) return FReply::Handled();
			
			Job->SetConfiguration(RenderingSettings->GetMasterConfig(Sequence, MoviePipelineQueue.Get()));
			Job->Map = QueueJob->Map;
			Job->JobName = QueueJob->JobName;
			Job->SetSequence(QueueJob->Sequence);
		}
	}

	UMoviePipelineExecutorBase* Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	if (!Executor) return FReply::Handled();
	Executor->OnExecutorFinished().AddLambda([](const UMoviePipelineExecutorBase* ExecutorBase, const bool bSuccess)
	{
		if (!bSuccess)
		{
			UE_LOG(LogGdhRenderingManager, Error, TEXT("Error occured when rendering images"));
			// GEditor->GetEditorSubsystem<UGdhSubsystem>()->ShowModalWithOutputLog(TEXT("GamedevHelper: Rendering Manager"), TEXT("Error Occured when rendering images"), EGdhModalStatus::Error, 5.0f);
			return;
		}

		UE_LOG(LogGdhRenderingManager, Error, TEXT("Rendering images finished successfully"));
		// GEditor->GetEditorSubsystem<UGdhSubsystem>()->ShowModal(TEXT("GamedevHelper: Rendering Manager"), TEXT("Rendering images finished successfully"), EGdhModalStatus::OK, 5.0f);
	});
	Executor->OnExecutorErrored().AddLambda([](const UMoviePipelineExecutorBase* PipelineExecutor, const UMoviePipeline* PipelineWithError, const bool bIsFatal, const FText ErrorText)
	{
		const FString ErrorMsg = FString::Printf(TEXT("Error occured with msg: %s"), *ErrorText.ToString());
		UE_LOG(LogGdhRenderingManager, Error, TEXT("%s"), *ErrorMsg);
		// GEditor->GetEditorSubsystem<UGdhSubsystem>()->ShowModalWithOutputLog(TEXT("GamedevHelper: Rendering Manager"), ErrorMsg, EGdhModalStatus::Error, 5.0f);
	});
	
	return FReply::Handled();
}

void SGdhRenderingManagerWindow::ListUpdate()
{
	if (!ListJobWidgetSwitcher) return;

	RenderingSettings->Validate();
	RenderingQueueSettings->Validate();

	if (!RenderingSettings->IsValid() || !RenderingQueueSettings->IsValid())
	{
		ListJobWidgetSwitcher->SetActiveWidgetIndex(0);
		return;
	}

	ListJobWidgetSwitcher->SetActiveWidgetIndex(1);

	ListItems.Reset();
	ListItems.Reserve(RenderingQueueSettings->LevelSequences.Num());

	for (const auto& LevelSequence : RenderingQueueSettings->LevelSequences)
	{
		UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
		if (!ListItem) continue;

		ListItem->LevelSequence = LevelSequence.LoadSynchronous();

		ListItems.Add(ListItem);
	}


	for (const auto& MoviePipelineQueue : RenderingQueueSettings->MoviePipelineQueues)
	{
		ListItems.Reserve(ListItems.Num() + MoviePipelineQueue->GetJobs().Num());

		for (const auto Job : MoviePipelineQueue->GetJobs())
		{
			UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
			if (!ListItem) continue;

			ListItem->LevelSequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			ListItem->MoviePipelineQueue = MoviePipelineQueue.LoadSynchronous();

			ListItems.Add(ListItem);
		}
	}

	if (ListView.IsValid())
	{
		ListView->RequestListRefresh();
	}
}
