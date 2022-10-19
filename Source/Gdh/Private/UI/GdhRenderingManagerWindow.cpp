// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "UI/GdhRenderingManagerListItem.h"
#include "Settings/GdhMovieRenderSettings.h"
#include "Settings/GdhRenderingSettings.h"
#include "Settings/GdhRenderingAssetsSettings.h"
// #include "Settings/GdhRenderingQueueSettings.h"
#include "GdhStyles.h"
#include "Libs/GdhNotificationLibrary.h"
#include "Libs/GdhRenderingLibrary.h"
// Engine Headers
#include "GdhCommands.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineCameraSetting.h"
#include "MoviePipelineConsoleVariableSetting.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineHighResSetting.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MoviePipelineWidgetRenderSetting.h"
#include "MovieSceneTimeHelpers.h"
#include "Misc/ScopedSlowTask.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	RenderingAssetsSettings = GetMutableDefault<UGdhRenderingAssetsSettings>();

	check(RenderingSettings);
	check(RenderingAssetsSettings);

	RenderingSettings->OnChange().BindLambda([&]()
	{
		ValidateSettings();
	});

	RenderingAssetsSettings->OnChange().BindLambda([&]()
	{
		ValidateSettings();
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

	DetailsViewArgs.ViewIdentifier = "GdhQueueSettings";
	const TSharedPtr<IDetailsView> RenderingAssetsSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingAssetsSettingsProperty->SetObject(RenderingAssetsSettings);

	ValidateSettings();
	// ListUpdate();

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
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Rendering Settings")))
								.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light20"))
							]
							+ SHorizontalBox::Slot()
							  .AutoWidth()
							  .Padding(FMargin{5.0f, 0.0f})
							[
								SNew(SBox)
								.WidthOverride(32.0f)
								.HeightOverride(32.0f)
								[
									SNew(SImage).Image_Lambda([&]()
									{
										const FString Status = bIsValidRenderingSettings ? TEXT("GamedevHelper.Icon.Check32") : TEXT("GamedevHelper.Icon.Cross32");
										return FGdhStyles::GetIcon(Status);
									})
								]
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							RenderingSettingsProperty.ToSharedRef()
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
							.Text(FText::FromString(TEXT("Rendering Assets")))
							.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light20"))
						]
						+ SVerticalBox::Slot()
						  .Padding(FMargin{5.0f, 0.0f})
						  .AutoHeight()
						[
							RenderingAssetsSettingsProperty.ToSharedRef()
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
					.Visibility_Raw(this, &SGdhRenderingManagerWindow::GetErrorMsgBoxVisibility)
					.VAlign(VAlign_Bottom)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Left)
						.Text_Raw(this, &SGdhRenderingManagerWindow::GetErrorMsgBoxText)
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

FText SGdhRenderingManagerWindow::GetErrorMsgBoxText() const
{
	return FText::FromString(ErrorMsg);
}

FText SGdhRenderingManagerWindow::GetJobStats() const
{
	return FText::FromString(JobStats);
}

EVisibility SGdhRenderingManagerWindow::GetErrorMsgBoxVisibility() const
{
	return ErrorMsg.IsEmpty() ? EVisibility::Hidden : EVisibility::Visible;
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
	// PluginCommands = MakeShareable(new FUICommandList);
	// PluginCommands->MapAction(
	// 	FGdhCommands::Get().Cmd_OpenImageDir,
	// 	FUIAction(
	// 		FExecuteAction::CreateLambda([&]()
	// 		{
	// 			if (!ListView.IsValid()) return;
	//
	// 			const auto SelectedItems = ListView->GetSelectedItems();
	// 			if (SelectedItems.Num() == 0) return;
	//
	// 			for (const auto& SelectedItem : SelectedItems)
	// 			{
	// 				if (!SelectedItem.IsValid()) break;
	// 				if (!SelectedItem->LevelSequence) break;
	//
	// 				const FString Path = UGdhRenderingLibrary::GetImageOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue);
	// 				if (!FPaths::DirectoryExists(Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
	// 					UGdhNotificationLibrary::ShowModalOutputLog(
	// 						TEXT("Rendering Manager"),
	// 						TEXT("Failed to open image output directory. Try to render jobs before doing this action"),
	// 						EGdhModalStatus::Error,
	// 						5.0f
	// 					);
	// 				}
	//
	// 				FPlatformProcess::ExploreFolder(*Path);
	// 			}
	// 		})
	// 	)
	// );
	// PluginCommands->MapAction(
	// 	FGdhCommands::Get().Cmd_OpenVideoDir,
	// 	FUIAction(
	// 		FExecuteAction::CreateLambda([&]()
	// 		{
	// 			if (!ListView.IsValid()) return;
	//
	// 			const auto SelectedItems = ListView->GetSelectedItems();
	// 			if (SelectedItems.Num() == 0) return;
	//
	// 			for (const auto& SelectedItem : SelectedItems)
	// 			{
	// 				if (!SelectedItem.IsValid()) break;
	// 				if (!SelectedItem->LevelSequence) break;
	//
	// 				const FString Path = UGdhRenderingLibrary::GetVideoOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue);
	// 				if (!FPaths::DirectoryExists(Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
	// 					UGdhNotificationLibrary::ShowModalOutputLog(TEXT(""),TEXT("Cant open video output directory for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
	// 					return;
	// 				}
	//
	// 				FPlatformProcess::ExploreFolder(*Path);
	// 			}
	// 		})
	// 	)
	// );
	// PluginCommands->MapAction(
	// 	FGdhCommands::Get().Cmd_PlayVideoFile,
	// 	FUIAction(
	// 		FExecuteAction::CreateLambda([&]()
	// 		{
	// 			if (!ListView.IsValid()) return;
	//
	// 			const auto SelectedItems = ListView->GetSelectedItems();
	// 			if (SelectedItems.Num() == 0) return;
	//
	// 			for (const auto& SelectedItem : SelectedItems)
	// 			{
	// 				if (!SelectedItem.IsValid()) break;
	// 				if (!SelectedItem->LevelSequence) break;
	//
	// 				const FString Path = FString::Printf(
	// 					TEXT("%s/%s.%s"),
	// 					*UGdhRenderingLibrary::GetVideoOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue),
	// 					*SelectedItem->LevelSequence->GetName(),
	// 					*UGdhRenderingLibrary::GetVideoExtension(RenderingSettings->VideoFormat, false)
	// 				);
	// 				if (!FPaths::FileExists(Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
	// 					UGdhNotificationLibrary::ShowModalOutputLog(TEXT(""), TEXT("Cant open video file for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
	// 				}
	//
	// 				FPlatformProcess::LaunchFileInDefaultExternalApplication(*Path);
	// 			}
	// 		})
	// 	)
	// );
	// PluginCommands->MapAction(
	// 	FGdhCommands::Get().Cmd_RemoveRenderedImages,
	// 	FUIAction(
	// 		FExecuteAction::CreateLambda([&]()
	// 		{
	// 			if (!ListView.IsValid()) return;
	//
	// 			const auto SelectedItems = ListView->GetSelectedItems();
	// 			if (SelectedItems.Num() == 0) return;
	//
	// 			for (const auto& SelectedItem : SelectedItems)
	// 			{
	// 				if (!SelectedItem.IsValid()) break;
	// 				if (!SelectedItem->LevelSequence) break;
	//
	// 				const FString Path = UGdhRenderingLibrary::GetImageOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue);
	// 				if (!FPaths::DirectoryExists(Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
	// 					// GdhSubsystem->ShowModalWithOutputLog(TEXT("Cant remove rendered images for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
	// 					return;
	// 				}
	//
	// 				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	//
	// 				if (!PlatformFile.DeleteDirectoryRecursively(*Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("Failed to remove %s directory"), *Path);
	// 					// GdhSubsystem->ShowModalWithOutputLog(TEXT("Failed to remove rendered images for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
	// 					return;
	// 				}
	//
	// 				PlatformFile.CreateDirectoryTree(*Path);
	// 			}
	//
	// 			UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), TEXT("Rendered images removed successfully"), EGdhModalStatus::OK, 3.0f);
	//
	// 			ListUpdate();
	// 		})
	// 	)
	// );
	// PluginCommands->MapAction(
	// 	FGdhCommands::Get().Cmd_RemoveEncodedVideo,
	// 	FUIAction(
	// 		FExecuteAction::CreateLambda([&]()
	// 		{
	// 			if (!ListView.IsValid()) return;
	//
	// 			const auto SelectedItems = ListView->GetSelectedItems();
	// 			if (SelectedItems.Num() == 0) return;
	//
	// 			for (const auto& SelectedItem : SelectedItems)
	// 			{
	// 				if (!SelectedItem.IsValid()) break;
	// 				if (!SelectedItem->LevelSequence) break;
	//
	// 				const FString Path = FString::Printf(
	// 					TEXT("%s/%s.%s"),
	// 					*UGdhRenderingLibrary::GetVideoOutputDirectoryPath(SelectedItem->LevelSequence, SelectedItem->MoviePipelineQueue),
	// 					*SelectedItem->LevelSequence->GetName(),
	// 					*UGdhRenderingLibrary::GetVideoExtension(RenderingSettings->VideoFormat)
	// 				);
	// 				if (!FPaths::FileExists(Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("%s does not exist"), *Path);
	// 					// GdhSubsystem->ShowModalWithOutputLog(TEXT("Cant remove encoded video file for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
	// 					return;
	// 				}
	//
	// 				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	//
	// 				if (!PlatformFile.DeleteFile(*Path))
	// 				{
	// 					UE_LOG(LogGdh, Error, TEXT("Failed to remove %s file"), *Path);
	// 					// GdhSubsystem->ShowModalWithOutputLog(TEXT("Failed to remove encoded video file for selected LevelSequence"), EGdhModalStatus::Error, 5.0f);
	// 					return;
	// 				}
	//
	// 				PlatformFile.CreateDirectoryTree(*FPaths::GetPath(Path));
	// 			}
	//
	// 			UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), TEXT("Encoded video removed successfully"), EGdhModalStatus::OK, 3.0f);
	//
	// 			ListUpdate();
	// 		})
	// 	)
	// );
}

void SGdhRenderingManagerWindow::ListUpdate()
{
	// if (!GEditor) return;
	// if (!bIsValidSettings) return;
	//
	// bCanStartRendering = false;
	// JobStats.Reset();
	// ConsoleBoxText.Reset();
	// ListItems.Reset();
	// ListItems.Reserve(QueueSettings->LevelSequences.Num() + QueueSettings->MoviePipelineQueues.Num());
	//
	// if (QueueSettings->LevelSequences.Num() == 0 && QueueSettings->MoviePipelineQueues.Num() == 0)
	// {
	// 	ConsoleBoxText = TEXT("Queue is empty. Select some LevelSequence or MoviePipelineQueue assets in order to render");
	// 	return;
	// }
	//
	// // FFmpegCommands.Reset();
	// // FFmpegCommands.Reserve(QueueSettings->LevelSequences.Num());
	// double TotalJobsDuration = 0;
	//
	// if (QueueSettings->LevelSequences.Num() > 0)
	// {
	// 	FScopedSlowTask LevelSequenceLoadingSlowTask{
	// 		static_cast<float>(QueueSettings->LevelSequences.Num()),
	// 		FText::FromString(TEXT("Loading LevelSequence assets..."))
	// 	};
	// 	LevelSequenceLoadingSlowTask.MakeDialog(false, false);
	//
	// 	for (const auto& LevelSequenceSetting : QueueSettings->LevelSequences)
	// 	{
	// 		LevelSequenceLoadingSlowTask.EnterProgressFrame(1.0f);
	//
	// 		ULevelSequence* LevelSequence = LevelSequenceSetting.Key.LoadSynchronous();
	// 		if (!LevelSequence)
	// 		{
	// 			ConsoleBoxText = TEXT("Failed to load some LevelSequence assets in queue");
	// 			return;
	// 		}
	//
	// 		UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
	// 		if (!ListItem) continue;
	//
	// 		ListItem->LevelSequence = LevelSequence;
	//
	// 		if (LevelSequenceSetting.Value.IsNull())
	// 		{
	// 			ListItem->Map = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
	// 		}
	// 		else
	// 		{
	// 			UWorld* Map = LevelSequenceSetting.Value.LoadSynchronous();
	// 			if (!Map)
	// 			{
	// 				ConsoleBoxText = FString::Printf(TEXT("Failed to load Map asset for %s LevelSequence"), *LevelSequence->GetName());
	// 				return;
	// 			}
	//
	// 			ListItem->Map = Map;
	// 		}
	//
	// 		TotalJobsDuration += UGdhRenderingLibrary::GetLevelSequenceDuration(LevelSequence) / RenderingSettings->Framerate.AsDecimal();
	//
	// 		ListItems.Add(ListItem);
	// 	}
	// }
	//
	// if (QueueSettings->MoviePipelineQueues.Num() > 0)
	// {
	// 	FScopedSlowTask MoviePipelineQueueLoadingSlowTask{
	// 		static_cast<float>(QueueSettings->MoviePipelineQueues.Num()),
	// 		FText::FromString(TEXT("Loading MoviePipelineQueue assets..."))
	// 	};
	// 	MoviePipelineQueueLoadingSlowTask.MakeDialog();
	//
	// 	for (const auto& MoviePipelineQueue : QueueSettings->MoviePipelineQueues)
	// 	{
	// 		MoviePipelineQueueLoadingSlowTask.EnterProgressFrame(1.0f);
	//
	// 		if (!MoviePipelineQueue.LoadSynchronous())
	// 		{
	// 			ConsoleBoxText = TEXT("Failed to load some MoviePipelineQueue assets");
	// 			return;
	// 		}
	//
	// 		const TArray<UMoviePipelineExecutorJob*> Jobs = MoviePipelineQueue->GetJobs();
	// 		if (Jobs.Num() == 0)
	// 		{
	// 			ConsoleBoxText = FString::Printf(TEXT("No jobs in %s asset"), *MoviePipelineQueue->GetName());
	// 			return;
	// 		}
	//
	// 		ListItems.Reserve(ListItems.Num() + Jobs.Num());
	//
	// 		FScopedSlowTask JobLoadingSlowTask{
	// 			static_cast<float>(Jobs.Num()),
	// 			FText::FromString(TEXT("Loading Jobs..."))
	// 		};
	// 		JobLoadingSlowTask.MakeDialog();
	//
	// 		for (const auto& Job : Jobs)
	// 		{
	// 			JobLoadingSlowTask.EnterProgressFrame(1.0f);
	//
	// 			if (!Job) continue;
	//
	// 			ULevelSequence* LevelSequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
	// 			UWorld* Map = Cast<UWorld>(Job->Map.TryLoad());
	//
	// 			// todo:ashe23 refactor this part
	// 			if (!LevelSequence)
	// 			{
	// 				ConsoleBoxText = FString::Printf(TEXT("%s asset contains job with invalid sequence"), *MoviePipelineQueue->GetName());
	// 				return;
	// 			}
	//
	// 			if (!Map)
	// 			{
	// 				ConsoleBoxText = FString::Printf(TEXT("%s asset contains job with invalid map"), *MoviePipelineQueue->GetName());
	// 				return;
	// 			}
	//
	// 			UGdhRenderingManagerListItem* ListItem = NewObject<UGdhRenderingManagerListItem>();
	// 			if (!ListItem) continue;
	// 			ListItem->Map = Map;
	// 			ListItem->LevelSequence = LevelSequence;
	// 			ListItem->MoviePipelineQueue = MoviePipelineQueue.Get();
	//
	// 			TotalJobsDuration += UGdhRenderingLibrary::GetLevelSequenceDuration(LevelSequence) / RenderingSettings->Framerate.AsDecimal();
	//
	// 			ListItems.Add(ListItem);
	// 		}
	// 	}
	// }
	//
	// if (ListView.IsValid())
	// {
	// 	ListView->RequestListRefresh();
	// }
	//
	// bCanStartRendering = ListItems.Num() > 0;
	// JobStats = FString::Printf(TEXT("%d %s (total duration %.1f seconds)"), ListItems.Num(), ListItems.Num() > 1 ? TEXT("jobs") : TEXT("job"), TotalJobsDuration);
}

void SGdhRenderingManagerWindow::ValidateRenderingSettings()
{
	if (!RenderingSettings) return;

	bIsValidRenderingSettings = false;
	ErrorMsg.Reset();

	if (RenderingSettings->OutputDirectory.Path.IsEmpty())
	{
		ErrorMsg = TEXT("Error: Output directory is not specified");
		return;
	}

	if (!FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path))
	{
		ErrorMsg = TEXT("Error: Output directory does not exist");
		return;
	}

	if (GetResolution().X % 2 != 0 || GetResolution().Y % 2 != 0)
	{
		ErrorMsg = TEXT("Error: Resolution dimensions must be power of 2, in order to ffmpeg encoder work correctly");
		return;
	}

	if (RenderingSettings->FFmpegExe.FilePath.IsEmpty())
	{
		ErrorMsg = TEXT("Error: FFmpegExe path is not specifed");
		return;
	}

	if (FMath::IsNearlyZero(RenderingSettings->Framerate.AsDecimal()))
	{
		ErrorMsg = TEXT("Error: Invalid framerate");
		return;
	}

	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(RenderingSettings->FFmpegExe.FilePath)) && !RenderingSettings->FFmpegExe.FilePath.ToLower().Equals(TEXT("ffmpeg.exe")))
	{
		ErrorMsg = TEXT("Error: Specified FFmpegExe path does not exist"); // maybe user installed ffmpeg cli in system PATHS, which available globally, in this case we wont return any errors
		return;
	}

	const UMoviePipelineMasterConfig* MasterConfig = CreateMasterConfig();
	if (!MasterConfig)
	{
		ErrorMsg = TEXT("Error: Failed to create MovieRender Master Config");
		return;
	}

	ErrorMsg = GetMasterConfigValidationMsg(MasterConfig);
	if (!ErrorMsg.IsEmpty())
	{
		return;
	}

	bIsValidRenderingSettings = true;
}

UMoviePipelineMasterConfig* SGdhRenderingManagerWindow::CreateMasterConfig() const
{
	UMoviePipelineMasterConfig* MasterConfig = NewObject<UMoviePipelineMasterConfig>();
	if (!MasterConfig) return nullptr;

	if (RenderingSettings->bSettingsAAEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineAntiAliasingSetting> AntiAliasingSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineAntiAliasingSetting::StaticClass()))
		{
			AntiAliasingSetting->SpatialSampleCount = RenderingSettings->SpatialSampleCount;
			AntiAliasingSetting->TemporalSampleCount = RenderingSettings->TemporalSampleCount;
			AntiAliasingSetting->bOverrideAntiAliasing = RenderingSettings->bOverrideAntiAliasing;
			AntiAliasingSetting->AntiAliasingMethod = RenderingSettings->AntiAliasingMethod;
			AntiAliasingSetting->RenderWarmUpCount = RenderingSettings->RenderWarmUpCount;
			AntiAliasingSetting->bUseCameraCutForWarmUp = RenderingSettings->bUseCameraCutForWarmUp;
			AntiAliasingSetting->EngineWarmUpCount = RenderingSettings->EngineWarmUpCount;
			AntiAliasingSetting->bRenderWarmUpFrames = RenderingSettings->bRenderWarmUpFrames;
		}
	}

	if (RenderingSettings->bSettingsConsoleVariablesEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineConsoleVariableSetting> ConsoleVariableSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineConsoleVariableSetting::StaticClass()))
		{
			ConsoleVariableSetting->ConsoleVariables = RenderingSettings->ConsoleVariables;
			ConsoleVariableSetting->StartConsoleCommands = RenderingSettings->StartConsoleCommands;
			ConsoleVariableSetting->EndConsoleCommands = RenderingSettings->EndConsoleCommands;
		}
	}

	if (RenderingSettings->bSettingsGameOverrideEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineGameOverrideSetting> GameOverrideSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineGameOverrideSetting::StaticClass()))
		{
			GameOverrideSetting->GameModeOverride = RenderingSettings->GameModeOverride;
			GameOverrideSetting->bCinematicQualitySettings = RenderingSettings->bCinematicQualitySettings;
			GameOverrideSetting->TextureStreaming = RenderingSettings->TextureStreaming;
			GameOverrideSetting->bUseLODZero = RenderingSettings->bUseLODZero;
			GameOverrideSetting->bDisableHLODs = RenderingSettings->bDisableHLODs;
			GameOverrideSetting->bUseHighQualityShadows = RenderingSettings->bUseHighQualityShadows;
			GameOverrideSetting->ShadowDistanceScale = RenderingSettings->ShadowDistanceScale;
			GameOverrideSetting->ShadowRadiusThreshold = RenderingSettings->ShadowRadiusThreshold;
			GameOverrideSetting->bOverrideViewDistanceScale = RenderingSettings->bOverrideViewDistanceScale;
			GameOverrideSetting->ViewDistanceScale = RenderingSettings->ViewDistanceScale;
			GameOverrideSetting->bFlushGrassStreaming = RenderingSettings->bFlushGrassStreaming;
		}
	}

	if (RenderingSettings->bSettingsUIEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineWidgetRenderer> WidgetRenderer = MasterConfig->FindOrAddSettingByClass(UMoviePipelineWidgetRenderer::StaticClass()))
		{
			WidgetRenderer->bCompositeOntoFinalImage = RenderingSettings->bCompositeOntoFinalImage;
		}
	}

	if (RenderingSettings->bSettingsBurnInEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineBurnInSetting> BurnInSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineBurnInSetting::StaticClass()))
		{
			BurnInSetting->BurnInClass = RenderingSettings->BurnInClass;
			BurnInSetting->bCompositeOntoFinalImage = RenderingSettings->bBurnInCompositeOntoFinalImage;
		}
	}

	if (RenderingSettings->bSettingsCameraEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineCameraSetting> CameraSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineCameraSetting::StaticClass()))
		{
			CameraSetting->OverscanPercentage = RenderingSettings->OverscanPercentage;
			CameraSetting->ShutterTiming = RenderingSettings->ShutterTiming;
		}
	}

	if (RenderingSettings->bSettingsHighResEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineHighResSetting> HighResSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineHighResSetting::StaticClass()))
		{
			HighResSetting->TileCount = RenderingSettings->TileCount;
			HighResSetting->TextureSharpnessBias = RenderingSettings->TextureSharpnessBias;
			HighResSetting->OverlapRatio = RenderingSettings->OverlapRatio;
			HighResSetting->bOverrideSubSurfaceScattering = RenderingSettings->bOverrideSubSurfaceScattering;
			HighResSetting->BurleySampleCount = RenderingSettings->BurleySampleCount;
		}
	}

	return MasterConfig;
}

FString SGdhRenderingManagerWindow::GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig)
{
	if (!MasterConfig) return TEXT("");

	for (const auto& Setting : MasterConfig->GetAllSettings())
	{
		Setting->ValidateState();

		if (Setting->GetValidationState() == EMoviePipelineValidationState::Errors || Setting->GetValidationState() == EMoviePipelineValidationState::Warnings)
		{
			const TArray<FText> ValidationResults = Setting->GetValidationResults();

			FString ValidateErrorText;

			for (const auto& ValidationResult : ValidationResults)
			{
				ValidateErrorText.Append(ValidationResult.ToString() + TEXT("\n"));
			}

			return ValidateErrorText.TrimStartAndEnd();
		}
	}

	return TEXT("");
}

FIntPoint SGdhRenderingManagerWindow::GetResolution() const
{
	if (!RenderingSettings) return FIntPoint::ZeroValue;
	
	switch (RenderingSettings->ResolutionPreset)
	{
		case EGdhResolutionPreset::Res360P: return GdhConstants::Resolution360P;
		case EGdhResolutionPreset::Res480P: return GdhConstants::Resolution480P;
		case EGdhResolutionPreset::Res720P: return GdhConstants::Resolution720P;
		case EGdhResolutionPreset::Res1080P: return GdhConstants::Resolution1080P;
		case EGdhResolutionPreset::Res1440P: return GdhConstants::Resolution1440P;
		case EGdhResolutionPreset::Res2160P: return GdhConstants::Resolution2160P;
		case EGdhResolutionPreset::ResCustom: return RenderingSettings->CustomResolution;
		default: return GdhConstants::Resolution1080P;
	}
}

void SGdhRenderingManagerWindow::ValidateSettings()
{
	ValidateRenderingSettings();
}

FReply SGdhRenderingManagerWindow::OnBtnRefreshClick()
{
	// ListUpdate();

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClick()
{
	// FFmpegCommands.Reset();
	// FFmpegCommands.Reserve(ListItems.Num());
	//
	// for (const auto& ListItem : ListItems)
	// {
	// 	const FString EncodeCommand = UGdhRenderingLibrary::GetFFmpegEncodeCmd(ListItem->LevelSequence, ListItem->MoviePipelineQueue);
	// 	if (!EncodeCommand.IsEmpty())
	// 	{
	// 		const FString Name = ListItem->MoviePipelineQueue ? ListItem->MoviePipelineQueue->GetName() + TEXT(":") + ListItem->LevelSequence->GetName() : ListItem->LevelSequence->GetName();
	// 		FFmpegCommands.Add(FGdhFFmpegCommand{Name, EncodeCommand});
	// 	}
	// 	
	// 	const FString VideoOutputDir = UGdhRenderingLibrary::GetVideoOutputDirectoryPath(ListItem->LevelSequence,  ListItem->MoviePipelineQueue);
	// 	if (!FPaths::DirectoryExists(VideoOutputDir))
	// 	{
	// 		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// 		PlatformFile.CreateDirectoryTree(*VideoOutputDir);
	// 	}
	// }
	//
	// UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	// if (!CustomQueue)
	// {
	// 	UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Rendering Manager"), TEXT("Failed to create queue for rendering"), EGdhModalStatus::Error, 5.0f);
	// 	return FReply::Handled();
	// }
	//
	// CustomQueue->DeleteAllJobs();
	//
	// for (const auto& ListItem : ListItems)
	// {
	// 	if (!ListItem.IsValid()) continue;
	//
	// 	UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
	// 	if (!Job)
	// 	{
	// 		UE_LOG(LogGdh, Error, TEXT("Failed to create job for custom queue"));
	// 		return FReply::Handled();
	// 	}
	// 	Job->Sequence = ListItem->LevelSequence;
	// 	Job->Map = ListItem->Map;
	// 	Job->JobName = ListItem->LevelSequence->GetName();
	// 	Job->SetConfiguration(MovieRenderSettings->CreateMasterConfig());
	//
	// 	UMoviePipelineMasterConfig* Config = Job->GetConfiguration();
	// 	if (!Config)
	// 	{
	// 		return FReply::Handled();
	// 	}
	//
	// 	Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
	// 	Config->FindOrAddSettingByClass(UGdhRenderingLibrary::GetImageClass(RenderingSettings));
	//
	// 	UMoviePipelineOutputSetting* OutputSetting = Cast<UMoviePipelineOutputSetting>(Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
	// 	if (!OutputSetting)
	// 	{
	// 		return FReply::Handled();
	// 	}
	//
	// 	const int32 FrameStart = ConvertFrameTime(
	// 		UE::MovieScene::DiscreteInclusiveLower(ListItem->LevelSequence->MovieScene->GetPlaybackRange()),
	// 		ListItem->LevelSequence->MovieScene->GetTickResolution(),
	// 		ListItem->LevelSequence->MovieScene->GetDisplayRate()
	// 	).FloorToFrame().Value;
	// 	const int32 FrameEnd = ConvertFrameTime(
	// 		UE::MovieScene::DiscreteExclusiveUpper(ListItem->LevelSequence->MovieScene->GetPlaybackRange()),
	// 		ListItem->LevelSequence->MovieScene->GetTickResolution(),
	// 		ListItem->LevelSequence->MovieScene->GetDisplayRate()
	// 	).FloorToFrame().Value;
	//
	// 	OutputSetting->OutputDirectory.Path = UGdhRenderingLibrary::GetImageOutputDirectoryPath(ListItem->LevelSequence, ListItem->MoviePipelineQueue);
	// 	OutputSetting->FileNameFormat = FString::Printf(TEXT("{sequence_name}_%0.3f_{frame_number_rel}"), RenderingSettings->Framerate.AsDecimal());
	// 	OutputSetting->OutputResolution = RenderingSettings->GetResolution();
	// 	OutputSetting->bUseCustomFrameRate = true;
	// 	OutputSetting->OutputFrameRate = RenderingSettings->Framerate;
	// 	OutputSetting->bOverrideExistingOutput = true;
	// 	OutputSetting->ZeroPadFrameNumbers = 4;
	// 	OutputSetting->FrameNumberOffset = 0;
	// 	OutputSetting->HandleFrameCount = 0;
	// 	OutputSetting->OutputFrameStep = 1;
	// 	OutputSetting->bUseCustomPlaybackRange = true;
	// 	OutputSetting->CustomStartFrame = FrameStart;
	// 	OutputSetting->CustomEndFrame = FrameEnd;
	// }
	//
	// const FTimespan RenderStartTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
	//
	// const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	// if (!Executor) return FReply::Handled();
	//
	// Executor->OnExecutorFinished().AddLambda([&](UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
	// {
	// 	const FTimespan RenderEndTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
	//
	// 	ListUpdate();
	//
	// 	if (!bSuccess)
	// 	{
	// 		// UGdhNotificationLibrary::ShowModalWithOutputLog(TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
	// 		return;
	// 	}
	//
	//
	// 	const FString SuccessText = FString::Printf(TEXT("Images rendered successfully in %d sec"), (RenderEndTime - RenderStartTime).GetSeconds());
	// 	UE_LOG(LogGdh, Log, TEXT("%s"), *SuccessText);
	// 	UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), SuccessText, EGdhModalStatus::OK, 5.0f);
	//
	// 	UGdhRenderingLibrary::RunFFmpegCommands(FFmpegCommands);
	// });
	// Executor->OnExecutorErrored().AddLambda([&](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
	// {
	// 	ListUpdate();
	//
	// 	// GdhSubsystem->ShowModalWithOutputLog(TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
	// 	UE_LOG(LogGdh, Error, TEXT("%s"), *ErrorText.ToString());
	// });

	return FReply::Handled();
}

bool SGdhRenderingManagerWindow::IsBtnRenderEnabled() const
{
	// if (const FString ErrorMsg = UGdhRenderingLibrary::Check(RenderingSettings); !ErrorMsg.IsEmpty())
	// {
	// 	
	// }

	return bCanStartRendering;
}

bool SGdhRenderingManagerWindow::IsBtnRefreshEnabled() const
{
	return bIsValidRenderingSettings; // todo:ashe23 not sure
}
