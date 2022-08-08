// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/Renderer/GamedevHelperRendererUI.h"
#include "UI/Renderer/GamedevHelperRendererSettings.h"
#include "UI/Renderer/GamedevHelperRendererQueueItemUI.h"
#include "UI/Renderer/GamedevHelperRendererQueueItem.h"
#include "UI/GamedevHelperEditorStyle.h"
// Engine Headers
#include "MoviePipelineQueue.h"
#include "MovieSceneTimeHelpers.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "AssetRegistryModule.h"
#include "MoviePipelineConsoleVariableSetting.h"
#include "MoviePipelineGameOverrideSetting.h"
#include "MoviePipelineHighResSetting.h"
#include "MoviePipelineNewProcessExecutor.h"
#include "MoviePipelineWidgetRenderSetting.h"
// #include "VirtualGamesNotificationManager.h"
#include "Misc/ScopedSlowTask.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

void SGamedevHelperRendererUI::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RendererSettings = GetMutableDefault<UGamedevHelperRendererSettings>();
	verify(RendererSettings);

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GamedevHelperRendererSettings";

	const TSharedPtr<IDetailsView> RendererSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RendererSettingsProperty->SetObject(RendererSettings);
	
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
							RendererSettingsProperty.ToSharedRef()
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
							.OnClicked_Raw(this, &SGamedevHelperRendererUI::OnBtnRefreshClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRendererUI::IsBtnRefreshEnabled)
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
							.OnClicked_Raw(this, &SGamedevHelperRendererUI::OnBtnRenderClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRendererUI::IsBtnRenderEnabled)
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
								.Text(FText::FromString(TEXT("Clean Output Dir")))
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
								SAssignNew(QueueList, SListView<TWeakObjectPtr<UGamedevHelperRendererQueueItem>>)
								.ListItemsSource(&Queue)
								.SelectionMode(ESelectionMode::None)
								.OnGenerateRow(this, &SGamedevHelperRendererUI::OnGenerateRow)
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
									+ SHeaderRow::Column(FName("PipelineName"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(250.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("PipelineNameColumn", "Pipeline"))
									]
									+ SHeaderRow::Column(FName("ShotName"))
									.HAlignCell(HAlign_Center)
									.VAlignCell(VAlign_Center)
									.HAlignHeader(HAlign_Center)
									.HeaderContentPadding(FMargin(10.0f))
									.FixedWidth(250.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SequenceNameColumn", "Level Sequence"))
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
			.Visibility_Raw(this, &SGamedevHelperRendererUI::GetConsoleBoxVisibility)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Left)
				.Text_Raw(this, &SGamedevHelperRendererUI::GetConsoleBoxText)
				.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("VirtualGames.Font.Light15"))
				.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
			]
		]
	];
}

FText SGamedevHelperRendererUI::GetConsoleBoxText() const
{
	// IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (RendererSettings->FFmpegExePath.FilePath.IsEmpty())
	{
		return FText::FromString(TEXT("Error: FFmpeg exe path not specified"));
	}

	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(RendererSettings->FFmpegExePath.FilePath)))
	{
		return FText::FromString(TEXT("Error: FFmpeg exe path does not exist"));
	}

	if (RendererSettings->OutputDir.Path.IsEmpty())
	{
		return FText::FromString(TEXT("Error: Output directory not specified"));
	}

	if (!FPaths::DirectoryExists(FPaths::ConvertRelativePathToFull(RendererSettings->OutputDir.Path)))
	{
		return FText::FromString(TEXT("Error: Output directory does not exist"));
	}

	return FText::FromString(TEXT(""));
}

EVisibility SGamedevHelperRendererUI::GetConsoleBoxVisibility() const
{
	return RendererSettings->IsValid() ? EVisibility::Hidden : EVisibility::Visible;
}

TSharedRef<ITableRow> SGamedevHelperRendererUI::OnGenerateRow(TWeakObjectPtr<UGamedevHelperRendererQueueItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperRendererQueueItemUI, OwnerTable).QueueItem(InItem);
}

FReply SGamedevHelperRendererUI::OnBtnRefreshClicked()
{
	if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
	{
		return FReply::Handled();
	}
	
	ListUpdateData();
	ListRefresh();

	return FReply::Handled();
}

FReply SGamedevHelperRendererUI::OnBtnRenderClicked()
{
	return FReply::Handled();
}

bool SGamedevHelperRendererUI::IsBtnRefreshEnabled() const
{
	return RendererSettings->IsValid();
}

bool SGamedevHelperRendererUI::IsBtnRenderEnabled() const
{
	return bCanStartRender && Queue.Num() > 0;
}

void SGamedevHelperRendererUI::ListUpdateData()
{
	bRequireReRender = false;
	bCanStartRender = true;

	Queue.Reset();
	Queue.Reserve(RendererSettings->MovieRenderQueueAssets.Num());

	const FAssetRegistryModule& AssetRegistry = FModuleManager::GetModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

	FScopedSlowTask PipelinesSlowTask{
    	static_cast<float>(RendererSettings->MovieRenderQueueAssets.Num()),
    	FText::FromString(TEXT("Preparing render queue..."))
    };
    PipelinesSlowTask.MakeDialog();
    	
	for (const auto& Pipeline : RendererSettings->MovieRenderQueueAssets)
	{
		const FString PipelineAssetName = Pipeline.GetAssetName();
		
		PipelinesSlowTask.EnterProgressFrame(1.0f, FText::FromString(FString::Printf(TEXT("Preparing %s"), *PipelineAssetName)));
		if (PipelinesSlowTask.ShouldCancel())
		{
			break;
		}
		
		const UMoviePipelineQueue* PipelineQueue = Cast<UMoviePipelineQueue>(Pipeline.TryLoad());
		if (!PipelineQueue) continue;

		FScopedSlowTask JobsSlowTask{
			static_cast<float>(PipelineQueue->GetJobs().Num()),
			FText::FromString(TEXT("Preparing sequences..."))
		};
		JobsSlowTask.MakeDialog();
		
		const TArray<UMoviePipelineExecutorJob*> Jobs = PipelineQueue->GetJobs();

		Queue.Reserve(Queue.Num() + Jobs.Num());
		
		for (const auto& Job : Jobs)
		{
			JobsSlowTask.EnterProgressFrame(1.0f);
			
			if (JobsSlowTask.ShouldCancel())
			{
				break;
			}
			
			UGamedevHelperRendererQueueItem* QueueItem = NewObject<UGamedevHelperRendererQueueItem>();
			QueueItem->Status = EGamedevHelperRendererStatus::OK;
			QueueItem->PipelineName = PipelineAssetName;
			// QueueItem->ShotOutputDir = RendererSettings->OutputDir.Path + TEXT("/image/") + PipelineAssetName;

			if (!Job)
			{
				QueueItem->Note = TEXT("Invalid job settings");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}
			if (Job->JobName.IsEmpty())
			{
				QueueItem->Note = TEXT("Shot name is empty");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}
			if (!Job->Sequence.IsValid())
			{
				QueueItem->Note = TEXT("Shot missing sequence");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}
			if (!Job->Map.IsValid())
			{
				QueueItem->Note = TEXT("Shot missing map");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;	
			}
			UMoviePipelineMasterConfig* MasterConfig = Job->GetConfiguration();
			if (!MasterConfig)
			{
				QueueItem->Note = TEXT("Shot contains invalid configs");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}
			const FAssetData ShotAssetData = AssetRegistry.Get().GetAssetByObjectPath(Job->Sequence.GetAssetPathName());
			const ULevelSequence* LevelSequence = Cast<ULevelSequence>(ShotAssetData.GetAsset());
			if (!LevelSequence)
			{
				QueueItem->Note = TEXT("Shot contains invalid asset");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}
			
			UMovieScene* MovieScene = LevelSequence->MovieScene;
			if (!MovieScene)
			{
				QueueItem->Note = TEXT("Shot contains invalid movie scene");
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				Queue.Add(QueueItem);
				bCanStartRender = false;
				continue;
			}

			const FString ShotName = Job->Sequence.GetAssetName();
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
			const double ShotDurationInSeconds = ShotDurationInFrames / RendererSettings->Framerate.AsDecimal();

			MovieScene->SetDisplayRate(RendererSettings->Framerate);
			MovieScene->PostEditChange();

			QueueItem->SequenceName = ShotName;
			// QueueItem->ShotDuration = FString::Printf(TEXT("%d frames (%.2f seconds)"), ShotDurationInFrames, ShotDurationInSeconds);
			// QueueItem->ShotDurationInFrames = ShotDurationInFrames;
			// QueueItem->ShotStartFrame = StartFrame;
			// QueueItem->ShotRenderedFrames = QueueItem->GetRenderedFramesNum();
			// QueueItem->AudioTracks = Pipeline.AudioTracks;
			// QueueItem->LevelSequence = Job->Sequence;
			// QueueItem->Map = Job->Map;
			// QueueItem->UserSettings = MasterConfig->GetUserSettings();

			// if (!QueueItem->IsRendered())
			// {
			// 	QueueItem->Status = EGamedevHelperRendererStatus::Warning;
			// 	QueueItem->Note = TEXT("Missing rendered images");
			// 	bRequireReRender = true;
			// }

			Queue.Add(QueueItem);
		}
	}
}

void SGamedevHelperRendererUI::ListRefresh() const
{
	if (QueueList.IsValid())
	{
		QueueList->RebuildList();
	}
}

#undef LOCTEXT_NAMESPACE
