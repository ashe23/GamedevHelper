// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "UI/GdhRenderingManagerListItem.h"
#include "Settings/GdhMovieRenderSettings.h"
#include "Settings/GdhRenderingSettings.h"
#include "Settings/GdhRenderingQueueSettings.h"
#include "GdhStyles.h"
#include "GdhSubsystem.h"
// Engine Headers
#include "EditorLevelUtils.h"
#include "MoviePipelineQueueSubsystem.h"
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
					[
						RenderingSettingsProperty.ToSharedRef()
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

	if (!FPaths::FileExists(RenderingSettings->FFmpegExe.FilePath))
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

	return FText::FromString(TEXT(""));
}

EVisibility SGdhRenderingManagerWindow::GetConsoleBoxVisibility() const
{
	return RenderingSettings->IsValidSettings() && GdhSubsystem->IsValidMasterConfig(MovieRenderSettings->CreateMasterConfig()) ? EVisibility::Hidden : EVisibility::Visible;
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
	ListItems.Reset();
	ListItems.Reserve(QueueSettings->LevelSequences.Num());

	FScopedSlowTask LevelSequenceSlowTask{
		static_cast<float>(QueueSettings->LevelSequences.Num()),
		FText::FromString(TEXT("Loading LevelSequences ..."))
	};
	LevelSequenceSlowTask.MakeDialog();

	for (const auto& LevelSequenceSetting : QueueSettings->LevelSequences)
	{
		LevelSequenceSlowTask.EnterProgressFrame(1.0f);
		if (LevelSequenceSlowTask.ShouldCancel())
		{
			break;
		}

		ULevelSequence* LevelSequence = LevelSequenceSetting.LevelSequence.LoadSynchronous();
		if (!LevelSequence) continue;


		UWorld* Map = LevelSequenceSetting.bUseEditorMap ? GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld() : LevelSequenceSetting.Map.LoadSynchronous();
		if (!Map) continue;

		UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
		if (!ListItem) continue;

		ListItem->LevelSequence = LevelSequence;
		ListItem->Map = Map;

		ListItems.Add(ListItem);
	}

	if (ListView.IsValid())
	{
		ListView->RequestListRefresh();
	}
}

FReply SGdhRenderingManagerWindow::OnBtnRefreshClick()
{
	ListUpdate();

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClick()
{
	return FReply::Handled();
}
