// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerUI.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueItemUI.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperProjectSettings.h"
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


void SGamedevHelperRenderingManagerUI::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RenderingSettings = GetMutableDefault<UGamedevHelperRenderingSettings>();
	verify(RenderingSettings);

	RenderingSettings->PostEditChange();

	RenderingManagerQueueSettings = GetMutableDefault<UGamedevHelperRenderingManagerQueueSettings>();
	verify(RenderingManagerQueueSettings);

	RenderingManagerQueueSettings->PostEditChange();

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
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerUI::OnBtnRenderClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerUI::IsBtnRenderEnabled)
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
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerUI::OnBtnCleanOutputDirClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerUI::IsBtnCleanOutputDirEnabled)
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
							.OnClicked_Raw(this, &SGamedevHelperRenderingManagerUI::OnBtnOpenOutputDirClicked)
							.IsEnabled_Raw(this, &SGamedevHelperRenderingManagerUI::IsBtnOpenOutputDirEnabled)
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
									// + SHeaderRow::Column(FName("SequenceRenderedFrames"))
									// .HAlignCell(HAlign_Center)
									// .VAlignCell(VAlign_Center)
									// .HAlignHeader(HAlign_Center)
									// .HeaderContentPadding(FMargin(10.0f))
									// .FixedWidth(120.0f)
									// [
									// 	SNew(STextBlock)
									// 	.Text(LOCTEXT("SequenceRenderedFramesColumn", "Rendered Frames"))
									// ]
									// + SHeaderRow::Column(FName("SequenceOutputDir"))
									// .HAlignCell(HAlign_Center)
									// .VAlignCell(VAlign_Center)
									// .HAlignHeader(HAlign_Center)
									// .HeaderContentPadding(FMargin(10.0f))
									// [
									// 	SNew(STextBlock)
									// 	.Text(LOCTEXT("SequenceOutputDirColumn", "Output Directory"))
									// ]
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

EVisibility SGamedevHelperRenderingManagerUI::GetConsoleBoxVisibility() const
{
	return RenderingSettings->IsValid() && RenderingManagerQueueSettings->IsValid() ? EVisibility::Hidden : EVisibility::Visible;
}

TSharedRef<ITableRow> SGamedevHelperRenderingManagerUI::OnGenerateRow(TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperRenderingManagerQueueItemUI, OwnerTable).QueueItem(InItem);
}

void SGamedevHelperRenderingManagerUI::ListUpdateData()
{
	Queue.Reset();
	Queue.Reserve(RenderingManagerQueueSettings->GetQueueItems().Num());
	
	Queue = RenderingManagerQueueSettings->GetQueueItems();
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

	RenderingManagerQueueSettings->Validate();
	
	ListUpdateData();
	ListRefresh();

	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerUI::OnBtnRenderClicked() const
{
	if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
	{
		return FReply::Handled();
	}

	RenderingManagerQueueSettings->Validate();
	
	if (!RenderingSettings->IsValid() || !RenderingManagerQueueSettings->IsValid())
	{
		return FReply::Handled();
	}

	GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue()->CopyFrom(RenderingManagerQueueSettings->GetCustomPipeline());
	const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	Executor->OnExecutorFinished().AddLambda([&](UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
	{
		if (!ExecutorBase) return;
		
		if (!bSuccess)
		{
			GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(GamedevHelperStandardText::RenderFail, 3.0f);
			return;
		}

		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModal(GamedevHelperStandardText::RenderSuccess, EGamedevHelperModalStatus::Success, 3.0f);
		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->RunFFmpegPythonScript();
	});
	Executor->OnExecutorErrored().AddLambda([](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
	{
		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(FString::Printf(TEXT("Renderer finished with errors: %s"),  *ErrorText.ToString()), 5.0f);
	});
	
	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerUI::OnBtnCleanOutputDirClicked() const
{
	if (!GEditor) return FReply::Handled();
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DeleteDirectoryRecursively(*RenderingSettings->GetSubDirProject()))
	{
		GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModalWithOutputLog(GamedevHelperStandardText::OutputDirFailedToClean, 3.0f);
		return FReply::Handled();
	}

	GEditor->GetEditorSubsystem<UGamedevHelperSubsystem>()->ShowModal(GamedevHelperStandardText::OutputDirSuccessClean, EGamedevHelperModalStatus::Success, 3.0f);
	
	return FReply::Handled();
}

FReply SGamedevHelperRenderingManagerUI::OnBtnOpenOutputDirClicked() const
{
	if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
	{
		return FReply::Handled();
	}

	if (FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path))
	{
		FPlatformProcess::ExploreFolder(*RenderingSettings->OutputDirectory.Path);
	}
	
	return FReply::Handled();
}

bool SGamedevHelperRenderingManagerUI::IsBtnRefreshEnabled() const
{
	return RenderingSettings->IsValid() && RenderingManagerQueueSettings->IsValid();
}

bool SGamedevHelperRenderingManagerUI::IsBtnRenderEnabled() const
{
	return RenderingSettings->IsValid() && RenderingManagerQueueSettings->IsValid();
}

bool SGamedevHelperRenderingManagerUI::IsBtnCleanOutputDirEnabled() const
{
	return FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path);
}

bool SGamedevHelperRenderingManagerUI::IsBtnOpenOutputDirEnabled() const
{
	return FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path);
}

#undef LOCTEXT_NAMESPACE
