// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerUI.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerSettings.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueItemUI.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueItem.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "GamedevHelperTypes.h"
// Engine Headers
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MovieSceneTimeHelpers.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "AssetRegistryModule.h"
#include "MoviePipelineGameOverrideSetting.h"
#include "MoviePipelineWidgetRenderSetting.h"
#include "Misc/ScopedSlowTask.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"


void SGamedevHelperRenderingManagerUI::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RenderingManagerSettings = GetMutableDefault<UGamedevHelperRenderingManagerSettings>();
	verify(RenderingManagerSettings);

	RenderingManagerQueueSettings = GetMutableDefault<UGamedevHelperRenderingManagerQueueSettings>();
	verify(RenderingManagerQueueSettings);

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GamedevHelperRenderingManagerSettings";

	const TSharedPtr<IDetailsView> RenderingManagerSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingManagerSettingsProperty->SetObject(RenderingManagerSettings);

	DetailsViewArgs.ViewIdentifier = "GamedevHelperRenderingManagerQueueSettings";
	const TSharedPtr<IDetailsView> RenderingManagerQueueProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingManagerQueueProperty->SetObject(RenderingManagerQueueSettings);
	
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
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(FMargin{10.0f})
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
						[
							RenderingManagerSettingsProperty.ToSharedRef()
						]
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin{10.0f})
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
						[
							RenderingManagerQueueProperty.ToSharedRef()
						]
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
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerUI::OnBtnRefreshClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerUI::IsBtnRefreshEnabled)
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
							// .OnClicked_Raw(this, &SGamedevHelperRendererUI::OnBtnRenderClicked)
							// .IsEnabled_Raw(this, &SGamedevHelperRendererUI::IsBtnRenderEnabled)
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
							// .OnClicked_Raw(this, &SVirtualGamesRendererUI::OnBtnCleanClicked)
							[
								SNew(STextBlock)
								.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light10"))
								.Text(FText::FromString(TEXT("Clean Output Directory")))
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
								SAssignNew(QueueList, SListView<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>)
								.ListItemsSource(&Queue)
								.SelectionMode(ESelectionMode::None)
								.OnGenerateRow(this, &SGamedevHelperRenderingManagerUI::OnGenerateRow)
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
									.FixedWidth(250.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("QueueNameColumn", "Queue"))
									]
									+ SHeaderRow::Column(FName("SequenceName"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(250.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceNameColumn", "Level Sequence"))
									]
									+ SHeaderRow::Column(FName("SequenceDuration"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(200.0f)
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
									.FixedWidth(120.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceRenderedFramesColumn", "Rendered Frames"))
									]
									+ SHeaderRow::Column(FName("SequenceOutputDir"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceOutputDirColumn", "Output Directory"))
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
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.Visibility_Raw(this, &SGamedevHelperRenderingManagerUI::GetConsoleBoxVisibility)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Left)
				.Text_Raw(this, &SGamedevHelperRenderingManagerUI::GetConsoleBoxText)
				.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("VirtualGames.Font.Light15"))
				.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
			]
		]
	];
}

FText SGamedevHelperRenderingManagerUI::GetConsoleBoxText() const
{
	if (!RenderingManagerSettings->IsValid())
	{
		return FText::FromString(RenderingManagerSettings->GetErrorText());
	}

	// if (!RenderingManagerSettings->IsValid())
	// {
	// 	return FText::FromString(RenderingManagerQueue->GetErrorText());
	// }
	
	return FText::FromString(TEXT(""));
}

EVisibility SGamedevHelperRenderingManagerUI::GetConsoleBoxVisibility() const
{
	return RenderingManagerSettings->IsValid() ? EVisibility::Hidden : EVisibility::Visible;
}

TSharedRef<ITableRow> SGamedevHelperRenderingManagerUI::OnGenerateRow(TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperRenderingManagerQueueItemUI, OwnerTable).QueueItem(InItem);
}

void SGamedevHelperRenderingManagerUI::ListUpdateData()
{
	// todo:ashe23 move standard texts to global namespace later
	bCanStartRender = true;
	
	Queue.Reset();
	Queue.Reserve(RenderingManagerQueueSettings->Queue.Num());

	FScopedSlowTask SlowTaskQueue{
	    static_cast<float>(RenderingManagerQueueSettings->Queue.Num()),
	    FText::FromString(TEXT("Preparing render queue..."))
    };
    SlowTaskQueue.MakeDialog();

	for (FGamedevHelperRenderingManagerQueueItemData& QueueItemData : RenderingManagerQueueSettings->Queue)
	{
		if (!QueueItemData.QueueAsset.IsValid())
		{
			continue;
		}

		
		SlowTaskQueue.EnterProgressFrame(1.0f, FText::FromString(FString::Printf(TEXT("Preparing %s"), *QueueItemData.QueueAsset.GetAssetName())));
		if (SlowTaskQueue.ShouldCancel())
		{
			break;
		}

		const UMoviePipelineQueue* MoviePipelineQueue = Cast<UMoviePipelineQueue>(QueueItemData.QueueAsset.TryLoad());
		if (!MoviePipelineQueue) continue;

		FScopedSlowTask SlowTaskJobs{
			static_cast<float>(MoviePipelineQueue->GetJobs().Num()),
			FText::FromString(GamedevHelperStandardText::PreparingQueueJobs)
		};
		SlowTaskJobs.MakeDialog();

		const TArray<UMoviePipelineExecutorJob*> Jobs = MoviePipelineQueue->GetJobs();

		Queue.Reserve(Queue.Num() + Jobs.Num());

		for (const auto& Job : Jobs)
		{
			SlowTaskJobs.EnterProgressFrame(1.0f);

			if (SlowTaskJobs.ShouldCancel())
			{
				break;
			}
			
			UGamedevHelperRenderingManagerQueueItem* QueueItem = NewObject<UGamedevHelperRenderingManagerQueueItem>();
			QueueItem->Status = EGamedevHelperRendererStatus::OK;
			QueueItem->QueueName = MoviePipelineQueue->GetName();
			QueueItem->SequenceOutputDir = RenderingManagerSettings->OutputDir.Path; // todo:ashe23 change later
			
			if (!Job)
			{
				QueueItem->Note = TEXT("Invalid Job Settings");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}

			if (!Job->Sequence.IsValid())
			{
				QueueItem->Note = TEXT("Missing LevelSequence");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}

			if (!Job->Map.IsValid())
			{
				QueueItem->Note = TEXT("Missing Map");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}

			const UMoviePipelineMasterConfig* MasterConfig = Job->GetConfiguration();
			if (!MasterConfig)
			{
				QueueItem->Note = TEXT("Invalid configs");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}

			const ULevelSequence* LevelSequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			if (!LevelSequence)
			{
				QueueItem->Note = TEXT("Could not load LevelSequence asset");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}

			

			UMovieScene* MovieScene = LevelSequence->MovieScene;
			const FFrameRate DisplayRate = MovieScene->GetDisplayRate();
			const int32 StartFrame = ConvertFrameTime(
				UE::MovieScene::DiscreteInclusiveLower(MovieScene->GetPlaybackRange()),
				MovieScene->GetTickResolution(),
				DisplayRate
			).FloorToFrame().Value;
			const int32 EndFrame = ConvertFrameTime(
				UE::MovieScene::DiscreteExclusiveUpper(MovieScene->GetPlaybackRange()),
				MovieScene->GetTickResolution(),
				DisplayRate
			).FloorToFrame().Value;

			const int32 ShotDurationInFrames = EndFrame - StartFrame; 
			const double ShotDurationInSeconds = ShotDurationInFrames / RenderingManagerSettings->Framerate.AsDecimal();

			MovieScene->SetDisplayRate(RenderingManagerSettings->Framerate);
			MovieScene->PostEditChange();

			QueueItem->SequenceName = LevelSequence->GetName();
			QueueItem->SequenceDuration = FString::Printf(TEXT("%d frames (%.2f seconds)"), ShotDurationInFrames, ShotDurationInSeconds);
			QueueItem->SequenceDurationInFrames = ShotDurationInFrames;
			QueueItem->SequenceStartFrame = StartFrame;
			QueueItem->SequenceRenderedFrames = 0; // todo:ashe23

			if (Job->JobName.IsEmpty())
			{
				QueueItem->Note = TEXT("Job name not specified");
				QueueItem->Status = EGamedevHelperRendererStatus::Warning;
			}
			
			Queue.Add(QueueItem);
		}

	}
}

void SGamedevHelperRenderingManagerUI::ListRefresh() const
{
	if (QueueList.IsValid())
	{
		QueueList->RebuildList();
	}
}

FReply SGamedevHelperRenderingManagerUI::OnBtnRefreshClicked()
{
	if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
	{
		return FReply::Handled();
	}

	ListUpdateData();
	ListRefresh();

	return FReply::Handled();
}

bool SGamedevHelperRenderingManagerUI::IsBtnRefreshEnabled() const
{
	return RenderingManagerSettings->IsValid();
}

#undef LOCTEXT_NAMESPACE
