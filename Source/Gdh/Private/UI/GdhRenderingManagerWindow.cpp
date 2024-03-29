﻿// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "UI/GdhRenderingManagerListItem.h"
#include "Settings/GdhRenderingSettings.h"
#include "Settings/GdhRenderingAssetsSettings.h"
#include "GdhStyles.h"
#include "GdhCommands.h"
#include "Libs/GdhTimeLibrary.h"
#include "Libs/GdhNotificationLibrary.h"
// Engine Headers
#include "IPythonScriptPlugin.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineCameraSetting.h"
#include "MoviePipelineConsoleVariableSetting.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineHighResSetting.h"
#include "MoviePipelineImageSequenceOutput.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MoviePipelineWidgetRenderSetting.h"
#include "MovieSceneTimeHelpers.h"
#include "Dom/JsonObject.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/ScopedSlowTask.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Tracks/MovieSceneSlomoTrack.h"
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
		ListUpdate();
	});

	RenderingAssetsSettings->OnChange().BindLambda([&]()
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

	DetailsViewArgs.ViewIdentifier = "GdhQueueSettings";
	const TSharedPtr<IDetailsView> RenderingAssetsSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingAssetsSettingsProperty->SetObject(RenderingAssetsSettings);

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
				.Text(FText::FromString(TEXT("Rendering in progress...")))
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
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Rendering Assets")))
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
										const FString Status = bIsValidRenderingAssetsSettings ? TEXT("GamedevHelper.Icon.Check32") : TEXT("GamedevHelper.Icon.Cross32");
										return FGdhStyles::GetIcon(Status);
									})
								]
							]
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
							  .FillWidth(1.0)
							  .HAlign(HAlign_Left)
							  .VAlign(VAlign_Center)
							[
								SNew(SButton)
								.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRefreshClick)
								.ToolTipText(FText::FromString(TEXT("Refresh jobs list")))
								[
									SNew(SImage)
									.Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Refresh"))
								]
							]
							+ SHorizontalBox::Slot()
							  .AutoWidth()
							  .HAlign(HAlign_Right)
							  .VAlign(VAlign_Center)
							[
								SNew(SButton)
								.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnOutputDirCleanClick)
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnOutputDirCleanEnabled)
								.ToolTipText(FText::FromString(TEXT("Clean Output Directory")))
								[
									SNew(SImage)
									.Image(FGdhStyles::GetIcon("GamedevHelper.Icon.RemoveFolder"))
								]
							]
							+ SHorizontalBox::Slot()
							  .AutoWidth()
							  .HAlign(HAlign_Right)
							  .VAlign(VAlign_Center)
							[
								SNew(SButton)
								.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnOutputDirOpenClick)
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnOutputDirOpenEnabled)
								.ToolTipText(FText::FromString(TEXT("Open Output Directory in Explorer")))
								[
									SNew(SImage)
									.Image(FGdhStyles::GetIcon("GamedevHelper.Icon.OpenFolder"))
								]
							]
							+ SHorizontalBox::Slot()
							  .AutoWidth()
							  .HAlign(HAlign_Right)
							  .VAlign(VAlign_Center)
							[
								SNew(SButton)
								.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
								.ContentPadding(FMargin{5})
								.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRenderClick)
								.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRenderEnabled)
								.ToolTipText(FText::FromString(TEXT("Render everything in the jobs list")))
								[
									SNew(SImage)
									.Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Render"))
								]
							]
							// + SHorizontalBox::Slot()
							//   .Padding(FMargin{0.0f, 0.0f, 5.0f, 0.0f})
							//   .AutoWidth()
							//   .HAlign(HAlign_Right)
							//   .VAlign(VAlign_Center)
							// [
							// 	SNew(SButton)
							// 	.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#32a854"))})
							// 	.ContentPadding(FMargin{5})
							// 	.OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnRenderClick)
							// 	.IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnRenderEnabled)
							// 	.ToolTipText(FText::FromString(TEXT("Render everything in the jobs list")))
							// 	[
							// 		SNew(STextBlock)
							// 		.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light10"))
							// 		.Text(FText::FromString(TEXT("Render")))
							// 	]
							// ]
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
							.OnMouseButtonDoubleClick(this, &SGdhRenderingManagerWindow::OnListItemDblClick)
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

void SGdhRenderingManagerWindow::RegisterCommands()
{
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_OpenImageDir,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				FPlatformProcess::ExploreFolder(*GetImageOutputDir(SelectedItem));
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return false;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return false;

				return FPaths::DirectoryExists(*GetImageOutputDir(SelectedItem));
			})

		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_OpenVideoDir,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				FPlatformProcess::ExploreFolder(*GetVideoOutputDir(SelectedItem));
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return false;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return false;

				return FPaths::DirectoryExists(*GetVideoOutputDir(SelectedItem));
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_PlayVideoFile,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				FPlatformProcess::LaunchFileInDefaultExternalApplication(*GetVideoFilePath(SelectedItem));
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return false;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return false;

				return FPaths::FileExists(*GetVideoFilePath(SelectedItem));
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_RemoveRenderedImages,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				const auto ReturnCode = UGdhNotificationLibrary::ShowDialogWindow(
					TEXT("Remove Rendered Images"),
					TEXT("Are you sure you want delete all rendered images for selected job?"),
					EAppMsgType::YesNo
				);

				if (ReturnCode != EAppReturnType::Yes) return;

				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				const FString OutputDir = GetImageOutputDir(SelectedItem);

				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				if (!PlatformFile.DeleteDirectoryRecursively(*OutputDir))
				{
					const FString Msg = FString::Printf(TEXT("Failed to delete rendered images for %s job"), *SelectedItem->Name);
					UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Rendering Manager"), Msg, EGdhModalStatus::Error, 5.0f);
					return;
				}

				ListUpdate();
				UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), TEXT("Rendered images removed successfully"), EGdhModalStatus::OK, 3.0f);
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return false;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return false;

				return FPaths::DirectoryExists(*GetImageOutputDir(SelectedItem));
			})
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_RemoveEncodedVideo,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				const auto ReturnCode = UGdhNotificationLibrary::ShowDialogWindow(
					TEXT("Remove Encoded Video"),
					TEXT("Are you sure you want delete encoded video file for selected job?"),
					EAppMsgType::YesNo
				);

				if (ReturnCode != EAppReturnType::Yes) return;

				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				if (!PlatformFile.DeleteFile(*GetVideoFilePath(SelectedItem)))
				{
					const FString Msg = FString::Printf(TEXT("Failed to delete encoded video file for %s job"), *SelectedItem->Name);
					UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Rendering Manager"), Msg, EGdhModalStatus::Error, 5.0f);
					return;
				}

				ListUpdate();
				UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), TEXT("Encoded video file removed successfully"), EGdhModalStatus::OK, 3.0f);
			}),
			FCanExecuteAction::CreateLambda([&]()
				{
					if (!ListView.IsValid()) return false;

					const auto SelectedItem = GetSelectedItemChecked();
					if (!SelectedItem) return false;

					return FPaths::FileExists(*GetVideoFilePath(SelectedItem));
				}
			)
		)
	);
	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_RenderSelected,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> Items;
				Items.Add(SelectedItem);

				// render images only
				ImagesRender(Items, true, false);
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return false;

				return GetSelectedItemChecked() != nullptr;
			})
		)
	);

	PluginCommands->MapAction(
		FGdhCommands::Get().Cmd_EncodeSelected,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return;

				const auto SelectedItem = GetSelectedItemChecked();
				if (!SelectedItem) return;

				TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> Items;
				Items.Add(SelectedItem);

				// encode videos only
				ImagesRender(Items, false, true);
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				if (!ListView.IsValid()) return false;

				return GetSelectedItemChecked() != nullptr;
			})
		)
	);
}

FText SGdhRenderingManagerWindow::GetJobStats() const
{
	return FText::FromString(JobStats);
}

FText SGdhRenderingManagerWindow::GetErrorMsgBoxText() const
{
	return FText::FromString(ErrorMsg);
}

EVisibility SGdhRenderingManagerWindow::GetErrorMsgBoxVisibility() const
{
	return ErrorMsg.IsEmpty() ? EVisibility::Hidden : EVisibility::Visible;
}

int32 SGdhRenderingManagerWindow::GetActiveWidgetIndex() const
{
	if (!GEditor) return WidgetIndexIdle;
	if (GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering()) return WidgetIndexWorking;

	return WidgetIndexIdle;
}

TSharedPtr<SHeaderRow> SGdhRenderingManagerWindow::GetHeaderRow() const
{
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(FName("Name"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Job Name")))
			.Text(FText::FromString(TEXT("Job Name")))
		]
		+ SHeaderRow::Column(FName("Duration"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("LevelSequence total duration")))
			.Text(FText::FromString(TEXT("Duration")))
		]
		+ SHeaderRow::Column(FName("FrameStart"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillWidth(0.25f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Starting frame number")))
			.Text(FText::FromString(TEXT("Start Frame")))
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
		+ SHeaderRow::Column(FName("VideoEncoded"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		  .FillWidth(0.3f)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Shows if video encoded or not")))
			.Text(FText::FromString(TEXT("Video")))
		]
		+ SHeaderRow::Column(FName("Note"))
		  .HAlignCell(HAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignHeader(HAlign_Center)
		  .HeaderContentPadding(FMargin(10.0f))
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Note")))
		];
}

TSharedPtr<SWidget> SGdhRenderingManagerWindow::ListCreateContextMenu() const
{
	FMenuBuilder MenuBuilder{true, PluginCommands};
	MenuBuilder.BeginSection(TEXT("ActionsRender"), FText::FromString(TEXT("Render And Encode")));
	{
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_RenderSelected);
		MenuBuilder.AddMenuEntry(FGdhCommands::Get().Cmd_EncodeSelected);
	}
	MenuBuilder.EndSection();
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

void SGdhRenderingManagerWindow::OnListItemDblClick(TWeakObjectPtr<UGdhRenderingManagerListItem> Item)
{
	if (!Item.IsValid() || !GEditor)
	{
		return;
	}

	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Item.Get()->LevelSequence.Get());
}

TSharedRef<ITableRow> SGdhRenderingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGdhRenderingManagerListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGdhRenderingManagerListItem, OwnerTable).ListItem(InItem);
}

FReply SGdhRenderingManagerWindow::OnBtnRefreshClick()
{
	ListUpdate();

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnRenderClick()
{
	ListUpdate();

	if (!bIsValidRenderingSettings || !bIsValidRenderingAssetsSettings) return FReply::Handled();

	ImagesRender(ListItems, RenderingSettings->bOverrideExistingRenders, RenderingSettings->bOverrideExistingRenders);

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnOutputDirCleanClick()
{
	const auto ReturnCode = UGdhNotificationLibrary::ShowDialogWindow(
		TEXT("Remove Output Directory"),
		TEXT("Are you sure you want delete everything in Output Directory?"),
		EAppMsgType::YesNo
	);

	if (ReturnCode != EAppReturnType::Yes) return FReply::Handled();

	const FString OutputDir = FPaths::ConvertRelativePathToFull(RenderingSettings->OutputDirectory.Path);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DeleteDirectoryRecursively(*OutputDir))
	{
		UGdhNotificationLibrary::ShowModalOutputLog(
			TEXT("Rendering Manager"),
			FString::Printf(TEXT("Failed to clean %s directory"), *OutputDir),
			EGdhModalStatus::Error,
			5.0f);
		return FReply::Handled();
	}

	PlatformFile.CreateDirectoryTree(*RenderingSettings->OutputDirectory.Path);

	UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"),TEXT("Output directory cleaned successfully"), EGdhModalStatus::OK, 5.0f);

	ListUpdate();

	return FReply::Handled();
}

FReply SGdhRenderingManagerWindow::OnBtnOutputDirOpenClick()
{
	FPlatformProcess::ExploreFolder(*RenderingSettings->OutputDirectory.Path);

	return FReply::Handled();
}

bool SGdhRenderingManagerWindow::IsBtnRenderEnabled() const
{
	return bIsValidRenderingSettings && bIsValidRenderingAssetsSettings && ListItems.Num() > 0;
}

bool SGdhRenderingManagerWindow::IsBtnOutputDirCleanEnabled() const
{
	return bIsValidRenderingSettings;
}

bool SGdhRenderingManagerWindow::IsBtnOutputDirOpenEnabled() const
{
	return bIsValidRenderingSettings;
}

void SGdhRenderingManagerWindow::ListUpdate()
{
	if (!GEditor) return;

	ValidateSettings();

	if (!bIsValidRenderingSettings || !bIsValidRenderingAssetsSettings) return;

	ListItems.Reset();
	ListItems.Reserve(RenderingAssetsSettings->LevelSequences.Num());

	double TotalJobsDuration = 0;

	for (const auto& LevelSequence : RenderingAssetsSettings->LevelSequences)
	{
		const UWorld* Map = LevelSequence.Value.LoadSynchronous() ? LevelSequence.Value.Get() : GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
		const auto ListItem = CreateListItem(LevelSequence.Key.Get(), Map);
		if (!ListItem.IsValid()) continue;

		TotalJobsDuration += ListItem->DurationInSeconds;
		ListItems.Add(ListItem);
	}

	for (const auto& MoviePipelineQueue : RenderingAssetsSettings->MoviePipelineQueues)
	{
		ListItems.Reserve(ListItems.Num() + MoviePipelineQueue->GetJobs().Num());

		for (const auto& Job : MoviePipelineQueue->GetJobs())
		{
			const auto ListItem = CreateListItem(
				Cast<ULevelSequence>(Job->Sequence.TryLoad()),
				Cast<UWorld>(Job->Map.TryLoad()),
				MoviePipelineQueue.Get()
			);
			if (!ListItem.IsValid()) continue;

			TotalJobsDuration += ListItem->DurationInSeconds;
			ListItems.Add(ListItem);
		}
	}

	if (ListView.IsValid())
	{
		ListView->RequestListRefresh();
		ListView->ClearSelection();
	}

	JobStats = FString::Printf(TEXT("%d %s (total duration %s)"), ListItems.Num(), ListItems.Num() > 1 ? TEXT("jobs") : TEXT("job"), *UGdhTimeLibrary::GetHumanReadableTime(TotalJobsDuration));
}

void SGdhRenderingManagerWindow::ValidateSettings()
{
	ListItems.Reset();

	ValidateRenderingSettings();

	if (!bIsValidRenderingSettings) return;

	ValidateRenderingAssetsSettings();
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
		ErrorMsg = TEXT("Error: Specified FFmpegExe path does not exist");
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

void SGdhRenderingManagerWindow::ValidateRenderingAssetsSettings()
{
	if (!GEditor) return;
	if (!RenderingAssetsSettings) return;

	bIsValidRenderingAssetsSettings = false;
	ErrorMsg.Reset();

	if (RenderingAssetsSettings->LevelSequences.Num() == 0 && RenderingAssetsSettings->MoviePipelineQueues.Num() == 0)
	{
		ErrorMsg = TEXT("Select assets in order to start rendering");
		return;
	}

	FScopedSlowTask SlowTaskLevelSequence{
		static_cast<float>(RenderingAssetsSettings->LevelSequences.Num()),
		FText::FromString(TEXT("Loading LevelSequence assets..."))
	};
	SlowTaskLevelSequence.MakeDialog(false, false);
	for (const auto& LevelSequence : RenderingAssetsSettings->LevelSequences)
	{
		SlowTaskLevelSequence.EnterProgressFrame(1.0f);

		if (!LevelSequence.Key.LoadSynchronous())
		{
			ErrorMsg = TEXT("Detected invalid LevelSequence asset in List");
			return;
		}

		const UWorld* Map = LevelSequence.Value.LoadSynchronous() ? LevelSequence.Value.Get() : GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
		if (!Map)
		{
			ErrorMsg = FString::Printf(TEXT("Error: Failed to load Map for %s"), *LevelSequence.Key.GetAssetName());
			return;
		}
	}

	FScopedSlowTask SlowTaskQueues{
		static_cast<float>(RenderingAssetsSettings->MoviePipelineQueues.Num()),
		FText::FromString(TEXT("Loading MoviePipelineQueue assets..."))
	};
	SlowTaskQueues.MakeDialog(false, false);
	for (const auto& MoviePipelineQueue : RenderingAssetsSettings->MoviePipelineQueues)
	{
		SlowTaskQueues.EnterProgressFrame(1.0f);

		if (!MoviePipelineQueue.LoadSynchronous())
		{
			ErrorMsg = TEXT("Detected invalid MoviePipelineQueue asset in list");
			return;
		}

		const TArray<UMoviePipelineExecutorJob*> Jobs = MoviePipelineQueue->GetJobs();
		if (Jobs.Num() == 0)
		{
			ErrorMsg = FString::Printf(TEXT("%s has no jobs in it. Specify at least 1 job to proceed"), *MoviePipelineQueue.GetAssetName());
			return;
		}

		FScopedSlowTask SlowTaskJobs{
			static_cast<float>(Jobs.Num()),
			FText::FromString(FString::Printf(TEXT("Loading %s assets..."), *MoviePipelineQueue.GetAssetName()))
		};
		SlowTaskJobs.MakeDialog(false, false);

		for (const auto& Job : Jobs)
		{
			SlowTaskJobs.EnterProgressFrame(1.0f);

			if (!Job) continue;

			const TSoftObjectPtr<ULevelSequence> JobSequence = Job->Sequence.TryLoad();
			const TSoftObjectPtr<UWorld> JobMap = Job->Map.TryLoad();

			if (JobSequence.IsNull())
			{
				ErrorMsg = FString::Printf(TEXT("%s contains job (%s) with invalid LevelSequence"), *MoviePipelineQueue.GetAssetName(), *Job->JobName);
				return;
			}

			if (JobMap.IsNull())
			{
				ErrorMsg = FString::Printf(TEXT("%s contains job (%s) with invalid Map"), *MoviePipelineQueue.GetAssetName(), *Job->JobName);
				return;
			}
		}
	}

	bIsValidRenderingAssetsSettings = true;
}

UGdhRenderingManagerListItem* SGdhRenderingManagerWindow::GetSelectedItemChecked() const
{
	const auto SelectedItems = ListView->GetSelectedItems();
	if (SelectedItems.Num() == 0) return nullptr;

	const auto SelectedItem = SelectedItems[0];
	if (!SelectedItem.IsValid()) return nullptr;

	return SelectedItem.Get();
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

UClass* SGdhRenderingManagerWindow::GetImageClass() const
{
	switch (RenderingSettings->ImageFormat)
	{
		case EGdhImageFormat::Png:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
		case EGdhImageFormat::Jpg:
			return UMoviePipelineImageSequenceOutput_JPG::StaticClass();
		case EGdhImageFormat::Bmp:
			return UMoviePipelineImageSequenceOutput_BMP::StaticClass();
		default:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
	}
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

FString SGdhRenderingManagerWindow::GetResolutionFolderName() const
{
	switch (RenderingSettings->ResolutionPreset)
	{
		case EGdhResolutionPreset::Res360P:
			return TEXT("360p");
		case EGdhResolutionPreset::Res480P:
			return TEXT("480p");
		case EGdhResolutionPreset::Res720P:
			return TEXT("720p");
		case EGdhResolutionPreset::Res1080P:
			return TEXT("1080p");
		case EGdhResolutionPreset::Res1440P:
			return TEXT("1440p");
		case EGdhResolutionPreset::Res2160P:
			return TEXT("2160p");
		case EGdhResolutionPreset::ResCustom:
			return FString::Printf(TEXT("%d_%d"), RenderingSettings->CustomResolution.X, RenderingSettings->CustomResolution.Y);
		default:
			return TEXT("1080p");
	}
}

FString SGdhRenderingManagerWindow::GetImageExtension(const bool IncludeDot) const
{
	switch (RenderingSettings->ImageFormat)
	{
		case EGdhImageFormat::Png:
			return IncludeDot ? TEXT(".png") : TEXT("png");
		case EGdhImageFormat::Jpg:
			return IncludeDot ? TEXT(".jpeg") : TEXT("jpeg");
		case EGdhImageFormat::Bmp:
			return IncludeDot ? TEXT(".bmp") : TEXT("bmp");
		default:
			return IncludeDot ? TEXT(".png") : TEXT("png");
	}
}

FString SGdhRenderingManagerWindow::GetVideoExtension(const bool IncludeDot) const
{
	switch (RenderingSettings->VideoFormat)
	{
		case EGdhVideoFormat::Mp4:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
		case EGdhVideoFormat::Mkv:
			return IncludeDot ? TEXT(".mkv") : TEXT("mkv");
		case EGdhVideoFormat::Avi:
			return IncludeDot ? TEXT(".avi") : TEXT("avi");
		default:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
	}
}

FString SGdhRenderingManagerWindow::GetImageOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	if (ListItem->MoviePipelineQueue.IsValid())
	{
		return FPaths::ConvertRelativePathToFull(
			FString::Printf(
				TEXT("%s/images/%s_%.3f_%s/%s/%s"),
				*RenderingSettings->OutputDirectory.Path,
				*GetResolutionFolderName(),
				RenderingSettings->Framerate.AsDecimal(),
				*GetImageExtension(),
				*ListItem->MoviePipelineQueue.GetAssetName(),
				*ListItem->LevelSequence.GetAssetName()
			)
		);
	}

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/images/%s_%.3f_%s/%s"),
			*RenderingSettings->OutputDirectory.Path,
			*GetResolutionFolderName(),
			RenderingSettings->Framerate.AsDecimal(),
			*GetImageExtension(),
			*ListItem->LevelSequence.GetAssetName()
		)
	);
}

FString SGdhRenderingManagerWindow::GetVideoOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	if (ListItem->MoviePipelineQueue.IsValid())
	{
		return FPaths::ConvertRelativePathToFull(
			FString::Printf(
				TEXT("%s/videos/%s_%.3f_%s/%s"),
				*RenderingSettings->OutputDirectory.Path,
				*GetResolutionFolderName(),
				RenderingSettings->Framerate.AsDecimal(),
				*GetVideoExtension(),
				*ListItem->MoviePipelineQueue.GetAssetName()
			)
		);
	}

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/videos/%s_%.3f_%s/%s"),
			*RenderingSettings->OutputDirectory.Path,
			*GetResolutionFolderName(),
			RenderingSettings->Framerate.AsDecimal(),
			*GetVideoExtension(),
			*ListItem->LevelSequence.GetAssetName()
		)
	);
}

FString SGdhRenderingManagerWindow::GetVideoFilePath(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/%s.%s"),
			*GetVideoOutputDir(ListItem),
			*ListItem->LevelSequence.GetAssetName(),
			*GetVideoExtension()
		)
	);
}

FString SGdhRenderingManagerWindow::GetVideoEncodeCmd(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	return FString::Printf(
		TEXT("%s -y -framerate %0.3f -i %s/%s_%%04d.%s -vf scale=%d:%d %s -r %0.3f %s"),
		*RenderingSettings->FFmpegExe.FilePath,
		RenderingSettings->Framerate.AsDecimal(),
		*GetImageOutputDir(ListItem),
		*ListItem->LevelSequence.GetAssetName(),
		*GetImageExtension(),
		GetResolution().X,
		GetResolution().Y,
		*UKismetStringLibrary::JoinStringArray(RenderingSettings->FFmpegFlags, TEXT(" ")),
		RenderingSettings->Framerate.AsDecimal(),
		*GetVideoFilePath(ListItem)
	);
}

void SGdhRenderingManagerWindow::CreateImageOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	const FString OutputDir = GetImageOutputDir(ListItem);
	if (OutputDir.IsEmpty() || FPaths::DirectoryExists(OutputDir)) return;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.CreateDirectoryTree(*OutputDir);
}

void SGdhRenderingManagerWindow::CreateVideoOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	const FString OutputDir = GetVideoOutputDir(ListItem);
	if (OutputDir.IsEmpty() || FPaths::DirectoryExists(OutputDir)) return;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.CreateDirectoryTree(*OutputDir);
}

void SGdhRenderingManagerWindow::RemoveImageOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	const FString OutputDir = GetImageOutputDir(ListItem);

	if (OutputDir.IsEmpty() || !FPaths::DirectoryExists(OutputDir)) return;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.DeleteDirectoryRecursively(*OutputDir);
}

void SGdhRenderingManagerWindow::RemoveVideoOutputDir(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	const FString OutputDir = GetVideoOutputDir(ListItem);

	if (OutputDir.IsEmpty() || !FPaths::DirectoryExists(OutputDir)) return;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.DeleteDirectoryRecursively(*OutputDir);
}

void SGdhRenderingManagerWindow::RemoveVideoFile(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	const FString VideoFile = GetVideoFilePath(ListItem);

	if (VideoFile.IsEmpty() || !FPaths::FileExists(VideoFile)) return;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.DeleteFile(*VideoFile);
}

bool SGdhRenderingManagerWindow::ContainsTimeDilationTrack(const ULevelSequence* LevelSequence) const
{
	if (!LevelSequence) return false;

	const auto Tracks = LevelSequence->GetMovieScene()->GetMasterTracks();
	for (const auto& Track : Tracks)
	{
		if (Cast<UMovieSceneSlomoTrack>(Track))
		{
			return true;
		}
	}

	return false;
}

void SGdhRenderingManagerWindow::CalculateRenderedFrames(const TWeakObjectPtr<UGdhRenderingManagerListItem>& ListItem) const
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	struct DirectoryVisitor : IPlatformFile::FDirectoryVisitor
	{
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory) return false;

			const FString BaseName = FPaths::GetBaseFilename(FilenameOrDirectory);
			const FString Extension = FPaths::GetExtension(FilenameOrDirectory);
			if (BaseName.StartsWith(RequiredBaseName) && Extension.Equals(RequiredExtension))
			{
				TArray<FString> Parts;
				BaseName.ParseIntoArray(Parts, TEXT("_"));
				Frames.Add(FCString::Atoi(*Parts.Last()));
			}

			return true;
		}

		FString RequiredBaseName;
		FString RequiredExtension;
		TArray<int32> Frames;
	};

	DirectoryVisitor Visitor;
	Visitor.RequiredBaseName = ListItem->LevelSequence.GetAssetName();
	Visitor.RequiredExtension = GetImageExtension();

	const FString Path = GetImageOutputDir(ListItem);
	if (Path.IsEmpty()) return;

	if (!PlatformFile.IterateDirectory(*Path, Visitor))
	{
		return;
	}

	if (Visitor.Frames.Num() == 0)
	{
		return;
	}

	Visitor.Frames.Sort();
	ListItem.Get()->bHasMissingFrames = false;

	for (int32 i = 0; i < Visitor.Frames.Num(); ++i)
	{
		if (Visitor.Frames[i] != i)
		{
			ListItem.Get()->bHasMissingFrames = true;
		}
	}

	ListItem.Get()->RenderedFramesNum = Visitor.Frames.Num();
}

void SGdhRenderingManagerWindow::ImagesRender(const TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>>& Items, const bool bForceRender, const bool bForceEncode)
{
	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue) return;

	CustomQueue->DeleteAllJobs();

	for (const auto& Item : Items)
	{
		if (!Item.IsValid()) continue;

		if (bForceRender || Item->bHasMissingFrames || Item->RenderedFramesNum == 0 || Item->RenderedFramesNum != Item->DurationInFrames && !Item->bHasTimeDilationTrack)
		{
			UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
			if (!Job) continue;

			Job->Sequence = Item->LevelSequence.ToSoftObjectPath();
			Job->Map = Item->Map.ToSoftObjectPath();
			Job->JobName = Item->LevelSequence.GetAssetName();
			Job->SetConfiguration(CreateMasterConfig());

			UMoviePipelineMasterConfig* Config = Job->GetConfiguration();
			if (!Config) continue;

			Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
			Config->FindOrAddSettingByClass(GetImageClass());

			UMoviePipelineOutputSetting* OutputSetting = Cast<UMoviePipelineOutputSetting>(Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
			if (!OutputSetting) continue;

			// removing old renders
			RemoveImageOutputDir(Item);
			CreateImageOutputDir(Item);

			OutputSetting->OutputDirectory.Path = GetImageOutputDir(Item);
			OutputSetting->FileNameFormat = TEXT("{sequence_name}_{frame_number_rel}");
			OutputSetting->OutputResolution = GetResolution();
			OutputSetting->bUseCustomFrameRate = true;
			OutputSetting->OutputFrameRate = RenderingSettings->Framerate;
			OutputSetting->bOverrideExistingOutput = true;
			OutputSetting->ZeroPadFrameNumbers = 4;
			OutputSetting->FrameNumberOffset = 0;
			OutputSetting->HandleFrameCount = 0;
			OutputSetting->OutputFrameStep = 1;
			OutputSetting->bUseCustomPlaybackRange = true;
			OutputSetting->CustomStartFrame = Item->FrameStart;
			OutputSetting->CustomEndFrame = Item->FrameEnd;
		}
	}

	FFmpegCommands.Reset();
	FFmpegCommands.Reserve(Items.Num());

	for (const auto& Item : Items)
	{
		if (bForceEncode || !FPaths::FileExists(GetVideoFilePath(Item)) && !bForceRender)
		{
			// removing old encoded video file
			CreateVideoOutputDir(Item);
			RemoveVideoFile(Item);

			FFmpegCommands.Add(FGdhFFmpegCommand{Item->Name, GetVideoEncodeCmd(Item)});
		}
	}

	JobsTotal = CustomQueue->GetJobs().Num();
	JobsFinished = 0;

	if (FFmpegCommands.Num() == 0 && JobsTotal == 0)
	{
		UGdhNotificationLibrary::ShowModal(TEXT("Rendering manager"), TEXT("No jobs to execute"), EGdhModalStatus::None, 2.0f);
		return;
	}

	if (JobsTotal > 0)
	{
		UMoviePipelinePIEExecutor* Executor = Cast<UMoviePipelinePIEExecutor>(
			GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass()));
		if (!Executor) return;

		RenderStartTime = FPlatformTime::Seconds();

		Executor->OnIndividualJobWorkFinished().AddLambda([&](FMoviePipelineOutputData OutputData)
		{
			if (OutputData.bSuccess)
			{
				JobsFinished++;
			}
		});

		Executor->OnExecutorFinished().AddLambda([&](UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
		{
			if (!bSuccess)
			{
				UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Rendering Manager"),TEXT("Error occured when rendering images"), EGdhModalStatus::Error, 5.0f);
				return;
			}

			const double ElapsedTime = FPlatformTime::Seconds() - RenderStartTime;
			const FString Msg = FString::Printf(TEXT("Rendering Done. Finished %d of %d jobs in %s"), JobsFinished, JobsTotal, *UGdhTimeLibrary::GetHumanReadableTime(ElapsedTime));
			const EGdhModalStatus Status = JobsTotal == JobsFinished ? EGdhModalStatus::OK : EGdhModalStatus::Error;
			UE_LOG(LogGdh, Warning, TEXT("%s"), *Msg);
			UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), Msg, Status, 5.0f);

			if (Status == EGdhModalStatus::OK)
			{
				RunFFmpegCommands();
			}

			ListUpdate();
		});
		Executor->OnExecutorErrored().AddLambda([&](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
		{
			UGdhNotificationLibrary::ShowModal(
				TEXT("Rendering Manager"),
				FString::Printf(TEXT("Rendering finished with errors %s : %s"), *ErrorText.ToString(), PipelineExecutor ? *PipelineExecutor->GetName() : TEXT("")),
				EGdhModalStatus::Error,
				5.0f
			);

			ListUpdate();
		});

		return;
	}

	RunFFmpegCommands();
	ListUpdate();
}

void SGdhRenderingManagerWindow::RunFFmpegCommands()
{
	if (FFmpegCommands.Num() == 0) return;

	const FString JsonFilePath = FString::Printf(TEXT("%s/%s"), *RenderingSettings->OutputDirectory.Path, *GdhConstants::FFmpegJsonCommandsFile);
	const TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());
	for (const auto& FFmpegCommand : FFmpegCommands)
	{
		RootObject->SetStringField(FFmpegCommand.SequenceName, FFmpegCommand.Command);
	}

	FString JsonStr;
	const TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);

	if (!FFileHelper::SaveStringToFile(JsonStr, *JsonFilePath))
	{
		const FString ErrMsg = FString::Printf(TEXT("Failed to export %s file"), *JsonFilePath);
		UE_LOG(LogGdh, Warning, TEXT("%s"), *ErrMsg);
		UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), ErrMsg, EGdhModalStatus::Error, 5.0f);
		return;
	}

	// this is for UI update
	FPlatformProcess::Sleep(0.5f);

	const FString PythonCmd = FString::Printf(TEXT("%s -queue %s"), *GdhConstants::FFmpegPythonFile, *JsonFilePath);
	IPythonScriptPlugin::Get()->ExecPythonCommand(*PythonCmd);
}

TWeakObjectPtr<UGdhRenderingManagerListItem> SGdhRenderingManagerWindow::CreateListItem(const ULevelSequence* LevelSequence, const UWorld* Map, const UMoviePipelineQueue* MoviePipelineQueue) const
{
	if (!LevelSequence || !Map) return nullptr;

	const TWeakObjectPtr<UGdhRenderingManagerListItem> ListItem = NewObject<UGdhRenderingManagerListItem>();

	LevelSequence->MovieScene->SetDisplayRate(RenderingSettings->Framerate);

	ListItem->Map = Map;
	ListItem->LevelSequence = LevelSequence;
	ListItem->MoviePipelineQueue = MoviePipelineQueue;

	const int32 FrameStart = ConvertFrameTime(
		UE::MovieScene::DiscreteInclusiveLower(LevelSequence->MovieScene->GetPlaybackRange()),
		LevelSequence->MovieScene->GetTickResolution(),
		LevelSequence->MovieScene->GetDisplayRate()
	).FloorToFrame().Value;
	const int32 FrameEnd = ConvertFrameTime(
		UE::MovieScene::DiscreteExclusiveUpper(LevelSequence->MovieScene->GetPlaybackRange()),
		LevelSequence->MovieScene->GetTickResolution(),
		LevelSequence->MovieScene->GetDisplayRate()
	).FloorToFrame().Value;

	CalculateRenderedFrames(ListItem);

	ListItem->Name = MoviePipelineQueue ? FString::Printf(TEXT("%s (%s)"), *LevelSequence->GetName(), *MoviePipelineQueue->GetName()) : LevelSequence->GetName();
	ListItem->DurationInFrames = FrameEnd - FrameStart;
	ListItem->DurationInSeconds = ListItem->DurationInFrames / RenderingSettings->Framerate.AsDecimal();
	ListItem->FrameStart = FrameStart;
	ListItem->FrameEnd = FrameEnd;
	ListItem->bHasTimeDilationTrack = ContainsTimeDilationTrack(LevelSequence);
	ListItem->bVideoEncoded = FPaths::FileExists(GetVideoFilePath(ListItem));
	ListItem->Note = TEXT("Done");
	ListItem->NoteStatus = EGdhGenericStatus::OK;

	if (ListItem->RenderedFramesNum == 0)
	{
		ListItem->Note = TEXT("Missing Rendered Images");
		ListItem->NoteStatus = EGdhGenericStatus::Error;
	}
	else if (ListItem->bHasMissingFrames)
	{
		ListItem->Note = TEXT("Rendered images are not sequential. Missing some frames");
		ListItem->NoteStatus = EGdhGenericStatus::Error;
	}
	else if (!ListItem->bVideoEncoded)
	{
		ListItem->Note = TEXT("Missing encoded video");
		ListItem->NoteStatus = EGdhGenericStatus::Error;
	}

	return ListItem;
}
