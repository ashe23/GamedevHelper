// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "UI/GdhRenderingManagerListItem.h"
#include "Settings/GdhMovieRenderSettings.h"
#include "Settings/GdhRenderingSettings.h"
#include "Settings/GdhRenderingQueueSettings.h"
#include "GdhStyles.h"
#include "Libs/GdhNotificationLibrary.h"
#include "Libs/GdhRenderingLibrary.h"
// Engine Headers
#include "GdhCommands.h"
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

	check(RenderingSettings);
	check(MovieRenderSettings);
	check(QueueSettings);

	RenderingSettings->GdhRenderingSettingsOnChangeDelegate.BindLambda([&]()
	{
		ValidateSettings();
	});

	MovieRenderSettings->GdhMovieRenderSettingsOnChangeDelegate.BindLambda([&]()
	{
		ValidateSettings();
	});
	
	QueueSettings->GdhQueueSettingsOnChangeDelegate.BindLambda([&]()
	{
		ListUpdate();
	});

	RegisterCommands();

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

	ValidateSettings();
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
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
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
				]
				+ SSplitter::Slot()
				.Value(0.3f)
				[
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
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
							  .Padding(FMargin{0.0f, 0.0f, 5.0f, 0.0f})
							  .AutoWidth()
							[
								SNew(SButton)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								// .ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRefreshClick)
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRefreshEnabled)
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
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRenderEnabled)
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
							.OnContextMenuOpening(this, &SGdhRenderingManagerWindow::ListCreateContextMenu)
							.OnGenerateRow(this, &SGdhRenderingManagerWindow::OnGenerateRow)
							// .OnMouseButtonDoubleClick(this, &SGamedevHelperRenderingManagerWindow::OnListDblClick)
							.HeaderRow(GetHeaderRow())
						]
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
					.VAlign(VAlign_Bottom)
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
	return FText::FromString(ConsoleBoxText);
}

FText SGdhRenderingManagerWindow::GetJobStats() const
{
	return FText::FromString(JobStats);
}

EVisibility SGdhRenderingManagerWindow::GetConsoleBoxVisibility() const
{
	return ConsoleBoxText.IsEmpty() ? EVisibility::Hidden : EVisibility::Visible;
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

TSharedPtr<SWidget> SGdhRenderingManagerWindow::ListCreateContextMenu() const
{
	FMenuBuilder MenuBuilder{true, PluginCommands};
	MenuBuilder.BeginSection(TEXT("ActionsOpen"), FText::FromString(TEXT("Open")));
	{
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenImageDir);
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_OpenVideoDir);
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_PlayVideoFile);
	}
	MenuBuilder.EndSection();
	MenuBuilder.BeginSection(TEXT("ActionsRemove"), FText::FromString(TEXT("Remove")));
	{
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_RemoveRenderedImages);
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_RemoveEncodedVideo);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void SGdhRenderingManagerWindow::RegisterCommands()
{
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_OpenImageDir,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItems = ListView->GetSelectedItems();
				if (SelectedItems.Num() == 0) return;

				for (const auto& SelectedItem : SelectedItems)
				{
					if (!SelectedItem.IsValid()) break;
					if (!SelectedItem->LevelSequence) break;

					const FString Path = UGdhRenderingLibrary::GetImageOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue);
					if (!FPaths::DirectoryExists(Path))
					{
						UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
						UGdhNotificationLibrary::ShowModalOutputLog(
							TEXT("Rendering Manager"),
							TEXT("Failed to open image output directory. Try to render jobs before doing this action"),
							EGdhModalStatus::Error,
							5.0f
						);
					}

					FPlatformProcess::ExploreFolder(*Path);
				}
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_OpenVideoDir,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItems = ListView->GetSelectedItems();
				if (SelectedItems.Num() == 0) return;

				for (const auto& SelectedItem : SelectedItems)
				{
					if (!SelectedItem.IsValid()) break;
					if (!SelectedItem->LevelSequence) break;

					const FString Path = UGdhRenderingLibrary::GetVideoOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue);
					if (!FPaths::DirectoryExists(Path))
					{
						UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
						UGdhNotificationLibrary::ShowModalOutputLog(TEXT(""),TEXT("Cant open video output directory for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
						return;
					}

					FPlatformProcess::ExploreFolder(*Path);
				}
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_PlayVideoFile,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItems = ListView->GetSelectedItems();
				if (SelectedItems.Num() == 0) return;

				for (const auto& SelectedItem : SelectedItems)
				{
					if (!SelectedItem.IsValid()) break;
					if (!SelectedItem->LevelSequence) break;

					const FString Path = FString::Printf(
						TEXT("%s/%s.%s"),
						*UGdhRenderingLibrary::GetVideoOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue),
						*SelectedItem->LevelSequence->GetName(),
						*RenderingSettings->GetVideoExtension()
					);
					if (!FPaths::FileExists(Path))
					{
						UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
						UGdhNotificationLibrary::ShowModalOutputLog(TEXT(""), TEXT("Cant open video file for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
					}

					FPlatformProcess::LaunchFileInDefaultExternalApplication(*Path);
				}
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_RemoveRenderedImages,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItems = ListView->GetSelectedItems();
				if (SelectedItems.Num() == 0) return;

				for (const auto& SelectedItem : SelectedItems)
				{
					if (!SelectedItem.IsValid()) break;
					if (!SelectedItem->LevelSequence) break;

					const FString Path = UGdhRenderingLibrary::GetImageOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue);
					if (!FPaths::DirectoryExists(Path))
					{
						UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
						// GdhSubsystem->ShowModalWithOutputLog(TEXT("Cant remove rendered images for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
						return;
					}

					IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

					if (!PlatformFile.DeleteDirectoryRecursively(*Path))
					{
						UE_LOG(LogGdh, Error, TEXT("Failed to remove %s directory"), *Path);
						// GdhSubsystem->ShowModalWithOutputLog(TEXT("Failed to remove rendered images for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
						return;
					}

					PlatformFile.CreateDirectoryTree(*Path);
				}

				UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), TEXT("Rendered images removed successfully"), EGdhModalStatus::OK, 3.0f);

				ListUpdate();
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_RemoveEncodedVideo,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItems = ListView->GetSelectedItems();
				if (SelectedItems.Num() == 0) return;

				for (const auto& SelectedItem : SelectedItems)
				{
					if (!SelectedItem.IsValid()) break;
					if (!SelectedItem->LevelSequence) break;

					const FString Path = FString::Printf(
						TEXT("%s/%s.%s"),
						*UGdhRenderingLibrary::GetVideoOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue),
						*SelectedItem->LevelSequence->GetName(),
						*RenderingSettings->GetVideoExtension()
					);
					if (!FPaths::FileExists(Path))
					{
						UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
						// GdhSubsystem->ShowModalWithOutputLog(TEXT("Cant remove encoded video file for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
						return;
					}

					IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

					if (!PlatformFile.DeleteFile(*Path))
					{
						UE_LOG(LogGdh, Error, TEXT("Failed to remove %s file"), *Path);
						// GdhSubsystem->ShowModalWithOutputLog(TEXT("Failed to remove encoded video file for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
						return;
					}

					PlatformFile.CreateDirectoryTree(*FPaths::GetPath(Path));
				}

				UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), TEXT("Encoded video removed successfully"), EGdhModalStatus::OK, 3.0f);

				ListUpdate();
			})
		)
	);
}

void SGdhRenderingManagerWindow::ListUpdate()
{
	if (!GEditor) return;
	if (!bIsValidSettings) return;

	bCanStartRendering = false;
	JobStats.Reset();
	ConsoleBoxText.Reset();
	ListItems.Reset();
	ListItems.Reserve(QueueSettings->LevelSequences.Num() + QueueSettings->MoviePipelineQueues.Num());
	
	if (QueueSettings->LevelSequences.Num() == 0 && QueueSettings->MoviePipelineQueues.Num() == 0)
	{
		ConsoleBoxText = TEXT("Queue is empty. Select some LevelSequence or MoviePipelineQueue assets in order to render");
		return;
	}
	
	// FFmpegCommands.Reset();
	// FFmpegCommands.Reserve(QueueSettings->LevelSequences.Num());
	double TotalJobsDuration = 0;
	
	if (QueueSettings->LevelSequences.Num() > 0)
	{
		FScopedSlowTask LevelSequenceLoadingSlowTask{
			static_cast<float>(QueueSettings->LevelSequences.Num()),
			FText::FromString(TEXT("Loading LevelSequence assets..."))
		};
		LevelSequenceLoadingSlowTask.MakeDialog();
		
		for (const auto& LevelSequenceSetting : QueueSettings->LevelSequences)
		{
			LevelSequenceLoadingSlowTask.EnterProgressFrame(1.0f);
			if (LevelSequenceLoadingSlowTask.ShouldCancel())
			{
				break;
			}
			
			ULevelSequence* LevelSequence = LevelSequenceSetting.Key.LoadSynchronous();
			if (!LevelSequence)
			{
				ConsoleBoxText = TEXT("Failed to load some LevelSequence assets in queue");
				return;
			}

			UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
			if (!ListItem) continue;
			
			ListItem->LevelSequence = LevelSequence;
			
			if (LevelSequenceSetting.Value.IsNull())
			{
				ListItem->Map = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
			}
			else
			{
				UWorld* Map = LevelSequenceSetting.Value.LoadSynchronous();
				if (!Map)
				{
					ConsoleBoxText = FString::Printf(TEXT("Failed to load Map asset for %s LevelSequence"), *LevelSequence->GetName());
					return;
				}
				
				ListItem->Map = Map;
			}
		
			TotalJobsDuration += UGdhRenderingLibrary::GetLevelSequenceDuration(LevelSequence) / RenderingSettings->Framerate.AsDecimal();
		
			ListItems.Add(ListItem);
		}
	}

	if (QueueSettings->MoviePipelineQueues.Num() > 0)
	{
		FScopedSlowTask MoviePipelineQueueLoadingSlowTask{
			static_cast<float>(QueueSettings->MoviePipelineQueues.Num()),
			FText::FromString(TEXT("Loading MoviePipelineQueue assets..."))
		};
		MoviePipelineQueueLoadingSlowTask.MakeDialog();
		
		for (const auto& MoviePipelineQueue : QueueSettings->MoviePipelineQueues)
		{
			if (!MoviePipelineQueue.LoadSynchronous())
			{
				ConsoleBoxText = TEXT("Failed to load some MoviePipelineQueue assets");
				return;
			}
			
			MoviePipelineQueueLoadingSlowTask.EnterProgressFrame(1.0f);
			if (MoviePipelineQueueLoadingSlowTask.ShouldCancel())
			{
				break;
			}
			
			const TArray<UMoviePipelineExecutorJob*> Jobs = MoviePipelineQueue->GetJobs();
			if (Jobs.Num() == 0)
			{
				ConsoleBoxText = FString::Printf(TEXT("No jobs in %s asset"), *MoviePipelineQueue->GetName());
				return;
			}
			
			ListItems.Reserve(ListItems.Num() + Jobs.Num());
			
			FScopedSlowTask JobLoadingSlowTask{
				static_cast<float>(Jobs.Num()),
				FText::FromString(TEXT("Loading Jobs..."))
			};
			JobLoadingSlowTask.MakeDialog();

			for (const auto& Job : Jobs)
			{
				JobLoadingSlowTask.EnterProgressFrame(1.0f);
				if (JobLoadingSlowTask.ShouldCancel())
				{
					break;
				}
				
				if (!Job) continue;

				ULevelSequence* LevelSequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
				UWorld* Map = Cast<UWorld>(Job->Map.TryLoad());

				// todo:ashe23 refactor this part
				if (!LevelSequence)
				{
					ConsoleBoxText = FString::Printf(TEXT("%s asset contains job with invalid sequence"), *MoviePipelineQueue->GetName());
					return;
				}
				
				if (!Map)
				{
					ConsoleBoxText = FString::Printf(TEXT("%s asset contains job with invalid map"), *MoviePipelineQueue->GetName());
					return;
				}

				UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
				if (!ListItem) continue;
				ListItem->Map = Map;
				ListItem->LevelSequence = LevelSequence;
				ListItem->MoviePipelineQueue = MoviePipelineQueue.Get();

				TotalJobsDuration += UGdhRenderingLibrary::GetLevelSequenceDuration(LevelSequence) / RenderingSettings->Framerate.AsDecimal();
				
				ListItems.Add(ListItem);
			}
		}
	}
	
	if (ListView.IsValid())
	{
		ListView->RequestListRefresh();
	}

	bCanStartRendering = ListItems.Num() > 0;
	JobStats = FString::Printf(TEXT("%d %s (total duration %.1f seconds)"), ListItems.Num(), ListItems.Num() > 1 ? TEXT("jobs") : TEXT("job"), TotalJobsDuration);
}

void SGdhRenderingManagerWindow::ValidateSettings()
{
	bIsValidSettings = false;
	ConsoleBoxText.Reset();
	
	if (RenderingSettings->OutputDirectory.Path.IsEmpty())
	{
		ConsoleBoxText = TEXT("Output directory not specified");
		return;
	}

	if (!FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path))
	{
		ConsoleBoxText = TEXT("Output directory does not exist");
		return;
	}

	if (RenderingSettings->FFmpegExe.FilePath.IsEmpty())
	{
		ConsoleBoxText = TEXT("FFmpeg.exe path not specified. Must be absolute path to exe, or just can be ffmpeg.exe if you have it in system PATHS");
		return;
	}

	if (!RenderingSettings->FFmpegExe.FilePath.ToLower().Equals(TEXT("ffmpeg.exe")) && !FPaths::FileExists(RenderingSettings->FFmpegExe.FilePath))
	{
		ConsoleBoxText = FString::Printf(TEXT("Cant find ffmpeg.exe at given '%s' location"), *RenderingSettings->FFmpegExe.FilePath);
		return;
	}

	if (RenderingSettings->GetResolution().X % 2 != 0 || RenderingSettings->GetResolution().Y % 2 != 0)
	{
		ConsoleBoxText = TEXT("Resolution must have dimensions divisible by 2");
		return;
	}

	const UMoviePipelineMasterConfig* MasterConfig = MovieRenderSettings->CreateMasterConfig();
	if (!UGdhRenderingLibrary::IsValidMasterConfig(MasterConfig))
	{
		ConsoleBoxText = UGdhRenderingLibrary::GetMasterConfigValidationMsg(MasterConfig);
		return;
	}

	bIsValidSettings = true;
}

FReply SGdhRenderingManagerWindow::OnBtnRefreshClick()
{
	ListUpdate();

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClick()
{
	FFmpegCommands.Reset();
	FFmpegCommands.Reserve(ListItems.Num());

	for (const auto& ListItem : ListItems)
	{
		const FString EncodeCommand = UGdhRenderingLibrary::GetFFmpegEncodeCmd(ListItem->LevelSequence, ListItem->MoviePipelineQueue);
		if (!EncodeCommand.IsEmpty())
		{
			const FString Name = ListItem->MoviePipelineQueue ? ListItem->MoviePipelineQueue->GetName() + TEXT(":") + ListItem->LevelSequence->GetName() : ListItem->LevelSequence->GetName();
			FFmpegCommands.Add(FGdhFFmpegCommand{Name, EncodeCommand});
		}
		
		const FString VideoOutputDir = UGdhRenderingLibrary::GetVideoOutputDirectoryPath(ListItem->LevelSequence,  ListItem->MoviePipelineQueue);
		if (!FPaths::DirectoryExists(VideoOutputDir))
		{
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			PlatformFile.CreateDirectoryTree(*VideoOutputDir);
		}
	}
	
	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue)
	{
		UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Rendering Manager"), TEXT("Failed to create queue for rendering"), EGdhModalStatus::Error, 5.0f);
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
	
		OutputSetting->OutputDirectory.Path = UGdhRenderingLibrary::GetImageOutputDirectoryPath(ListItem->LevelSequence, ListItem->MoviePipelineQueue);
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
	
		if (!bSuccess)
		{
			// UGdhNotificationLibrary::ShowModalWithOutputLog(TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
			return;
		}
	
	
		const FString SuccessText = FString::Printf(TEXT("Images rendered successfully in %d sec"), (RenderEndTime - RenderStartTime).GetSeconds());
		UE_LOG(LogGdh, Log, TEXT("%s"), *SuccessText);
		UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), SuccessText, EGdhModalStatus::OK, 5.0f);
	
		UGdhRenderingLibrary::RunFFmpegCommands(FFmpegCommands);
	});
	Executor->OnExecutorErrored().AddLambda([&](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
	{
		ListUpdate();
	
		// GdhSubsystem->ShowModalWithOutputLog(TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
		UE_LOG(LogGdh, Error, TEXT("%s"), *ErrorText.ToString());
	});
	
	return FReply::Handled();
}

bool SGdhRenderingManagerWindow::IsBtnRenderEnabled() const
{
	return bCanStartRendering;
}

bool SGdhRenderingManagerWindow::IsBtnRefreshEnabled() const
{
	return bIsValidSettings;
}
