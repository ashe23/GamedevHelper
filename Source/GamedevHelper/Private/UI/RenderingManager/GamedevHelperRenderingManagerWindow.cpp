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
#include "MoviePipelineGameOverrideSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineWidgetRenderSetting.h"
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
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.Visibility_Raw(this, &SGamedevHelperRenderingManagerWindow::GetConsoleBoxVisibility)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Left)
				.Text_Raw(this, &SGamedevHelperRenderingManagerWindow::GetConsoleBoxText)
				.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("VirtualGames.Font.Light15"))
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

void SGamedevHelperRenderingManagerWindow::ListUpdateData()
{
	if (!GEditor) return;
	
	ListItems.Reset();
	ListItems.Reserve(RenderingManagerQueueSettings->QueueAssets.Num());

	for (const auto& QueueAsset : RenderingManagerQueueSettings->QueueAssets)
	{
		const TObjectPtr<UMoviePipelineQueue> QueueAssetData = Cast<UMoviePipelineQueue>(QueueAsset.TryLoad());
		if (!QueueAssetData) continue;

		const TArray<UMoviePipelineExecutorJob*> QueueJobs = QueueAssetData->GetJobs();
		ListItems.Reserve(ListItems.Num() + QueueJobs.Num());
		
		for (const auto& Job : QueueJobs)
		{
			const TObjectPtr<UGamedevHelperRenderingManagerListItem> NewListItem = NewObject<UGamedevHelperRenderingManagerListItem>();
			NewListItem->Status = EGamedevHelperRendererStatus::OK;
			NewListItem->QueueName = QueueAsset.GetAssetName();
			NewListItem->QueueAsset = QueueAsset;
			
			if (!Job)
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Error;
				NewListItem->Note = TEXT("Contains Invalid Job");
				ListItems.Add(NewListItem);
				continue;
			}

			const TSoftObjectPtr<ULevelSequence> LevelSequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			if (!LevelSequence)
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Error;
				NewListItem->Note = TEXT("Contains invalid LevelSequence");
				ListItems.Add(NewListItem);
				continue;
			}

			if (!Job->Map.IsValid())
			{
				NewListItem->Status = EGamedevHelperRendererStatus::Error;
				NewListItem->Note = TEXT("Map not specified");
				ListItems.Add(NewListItem);
				continue;
			}

			NewListItem->LevelSequence = LevelSequence.Get();
			NewListItem->SequenceName = LevelSequence->GetName();

			FGamedevHelperSequencePlaybackInfo PlaybackInfo;
			GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->GetLevelSequencePlaybackInfo(LevelSequence, PlaybackInfo);
			
			NewListItem->SequenceDuration = FString::Printf(TEXT("%d frames (%.2f seconds)"), PlaybackInfo.DurationInFrames, PlaybackInfo.DurationInSeconds);
			NewListItem->SequenceDurationInFrames = PlaybackInfo.DurationInFrames;
			NewListItem->SequenceStartFrame = PlaybackInfo.FrameStart;
			NewListItem->SequenceRenderedFrames = 0; // todo:ashe23 implement later

			// todo:ashe23 add check if we need re render

			ListItems.Add(NewListItem);
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

FReply SGamedevHelperRenderingManagerWindow::OnBtnRefreshClicked()
{
	if (IsMovieRenderWorking()) return FReply::Handled();

	ListUpdateData();
	ListRefresh();

	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerWindow::OnBtnRenderClicked() const
{
	if (IsMovieRenderWorking()) return FReply::Handled();
	
	if (GEditor)
	{
		TArray<TSoftObjectPtr<UMoviePipelineQueue>> Queue;
		Queue.Reserve(RenderingManagerQueueSettings->QueueAssets.Num());

		for (const auto& QueueElem : RenderingManagerQueueSettings->QueueAssets)
		{
			Queue.Add(QueueElem.TryLoad());
		}
		
		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->RenderMovieRenderQueue(Queue);
	}

	
	// RenderingManagerQueueSettings->Validate();
	//
	// if (!RenderingSettings->IsValid() || !RenderingManagerQueueSettings->IsValid())
	// {
	// 	return FReply::Handled();
	// }

	// GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue()->CopyFrom(RenderingManagerQueueSettings->GetCustomPipeline());
	// const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	// Executor->OnExecutorFinished().AddLambda([&](UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
	// {
	// 	if (!ExecutorBase) return;
	// 	
	// 	if (!bSuccess)
	// 	{
	// 		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(GamedevHelperStandardText::RenderFail, 3.0f);
	// 		return;
	// 	}
	//
	// 	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModal(GamedevHelperStandardText::RenderSuccess, EGamedevHelperModalStatus::Success, 3.0f);
	// 	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->RunFFmpegPythonScript();
	// });
	// Executor->OnExecutorErrored().AddLambda([](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
	// {
	// 	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(FString::Printf(TEXT("Renderer finished with errors: %s"),  *ErrorText.ToString()), 5.0f);
	// });
	
	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerWindow::OnBtnCleanOutputDirClicked() const
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
