// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManagerWindow.h"

#include "GdhCoreStyles.h"
#include "GdhRenderingSettings.h"
#include "GdhRenderingQueueSettings.h"
#include "GdhRenderingManagerWindowListItem.h"
// Engine Headers
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
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
					.WidgetIndex(WidgetSwitcherNoJobsIndex)
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
								// .ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRefreshClicked)
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRefreshEnabled)
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
								.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#FF5722"))})
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRenderClicked)
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRefreshEnabled)
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

bool SGdhRenderingManagerWindow::IsMovieRenderWorking()
{
	if (!GEditor) return false;

	return GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClicked() const
{
	if (IsMovieRenderWorking()) return FReply::Handled();

	if (GdhDelegateRenderRequested.IsBound())
	{
		GdhDelegateRenderRequested.Execute();
	}

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnRefreshClicked()
{
	ListUpdate();

	return FReply::Handled();
}

bool SGdhRenderingManagerWindow::IsBtnRenderEnabled() const
{
	return RenderingSettings->IsValid() && RenderingQueueSettings->IsValid();
}

bool SGdhRenderingManagerWindow::IsBtnRefreshEnabled() const
{
	return RenderingSettings->IsValid() && RenderingQueueSettings->IsValid();
}

void SGdhRenderingManagerWindow::ListUpdate()
{
	if (ListJobWidgetSwitcher)
	{
		const int32 WidgetIndex = RenderingQueueSettings->LevelSequences.Num() > 0 || RenderingQueueSettings->MoviePipelineQueues.Num() > 0 ? WidgetSwitcherListIndex : WidgetSwitcherNoJobsIndex;
		ListJobWidgetSwitcher->SetActiveWidgetIndex(WidgetIndex);
	}

	ListItems.Reset();

	RenderingSettings->Validate();
	RenderingQueueSettings->Validate();

	if (!RenderingSettings->IsValid() || !RenderingQueueSettings->IsValid())
	{
		return;
	}

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
