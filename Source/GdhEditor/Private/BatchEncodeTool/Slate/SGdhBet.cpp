// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBet.h"
#include "BatchEncodeTool/GdhBetSettings.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
// #include "GdhCmds.h"
// #include "GdhConstants.h"
#include "GdhCmds.h"
#include "GdhLibEditor.h"
#include "GdhStyles.h"
#include "IContentBrowserSingleton.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
#include "SDropTarget.h"
#include "BatchEncodeTool/Slate/SGdhBetQueueItem.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "Kismet/KismetStringLibrary.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
// #include "Widgets/Layout/SSeparator.h"

void SGdhBet::Construct(const FArguments& InArgs) {

	Settings = GetMutableDefault<UGdhBetSettings>();
	if (!Settings) return;

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(FGdhCmds::Get().BetRefresh, FExecuteAction::CreateRaw(this, &SGdhBet::OnQueueRefresh));
	Cmds->MapAction(FGdhCmds::Get().BetProcess, FExecuteAction::CreateRaw(this, &SGdhBet::OnQueueProcess));

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhBetSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(Settings);

	FARFilter FilterRenderList;
	FARFilter FilterEncodePreset;

	FilterRenderList.ClassNames.Add(UGdhBetRenderList::StaticClass()->GetFName());

	FAssetPickerConfig PickerConfigRenderList;
	PickerConfigRenderList.bAllowNullSelection = true;
	PickerConfigRenderList.bAllowDragging = true;
	PickerConfigRenderList.Filter = FilterRenderList;
	PickerConfigRenderList.bAddFilterUI = false;
	PickerConfigRenderList.InitialAssetViewType = EAssetViewType::Column;
	PickerConfigRenderList.OnAssetDoubleClicked.BindStatic(&SGdhBet::OnRenderListDblClick);

	const auto BrowserRenderList = UGdhLibEditor::GetModuleContentBrowser().Get().CreateAssetPicker(PickerConfigRenderList);

	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot().Value(0.3f)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.ColorAndOpacity(FGdhStyles::Get().GetColor("GamedevHelper.Color.Gray"))
							.ShadowOffset(FVector2D{1.5f, 1.5f})
							.ShadowColorAndOpacity(FLinearColor::Black)
							.Font(FGdhStyles::GetFont("Bold", 15))
							.Text(FText::FromString(TEXT("General Settings")))
						]
					]
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SettingsProperty
					]
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.ColorAndOpacity(FGdhStyles::Get().GetColor("GamedevHelper.Color.Gray"))
							.ShadowOffset(FVector2D{1.5f, 1.5f})
							.ShadowColorAndOpacity(FLinearColor::Black)
							.Font(FGdhStyles::GetFont("Bold", 12))
							.Text(FText::FromString(TEXT("Encode Presets")))
						]
					]
				]
			]
			+ SSplitter::Slot().Value(0.3f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FGdhStyles::Get().GetColor("GamedevHelper.Color.Gray"))
						.ShadowOffset(FVector2D{1.5f, 1.5f})
						.ShadowColorAndOpacity(FLinearColor::Black)
						.Font(FGdhStyles::GetFont("Bold", 12))
						.Text(FText::FromString(TEXT("Render List")))
					]
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					BrowserRenderList
				]
			]
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FGdhStyles::Get().GetColor("GamedevHelper.Color.Gray"))
						.ShadowOffset(FVector2D{1.5f, 1.5f})
						.ShadowColorAndOpacity(FLinearColor::Black)
						.Font(FGdhStyles::GetFont("Bold", 12))
						.Text(FText::FromString(TEXT("Queue")))
					]
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					CreateToolbarQueue()
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SSeparator).Thickness(3.0f)
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SDropTarget)
					.OnDrop(this, &SGdhBet::OnDragDropTarget)
					.OnAllowDrop(this, &SGdhBet::CanDragDropTarget)
					.OnIsRecognized(this, &SGdhBet::CanDragDropTarget)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
						[
							SNew(SScrollBox)
							.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
							.AnimateWheelScrolling(true)
							.AllowOverscroll(EAllowOverscroll::No)
							+ SScrollBox::Slot()
							[
								SAssignNew(QueueView, SListView<TWeakObjectPtr<UGdhBetQueueItem>>)
								.ListItemsSource(&QueueItems)
								.SelectionMode(ESelectionMode::Multi)
								.ClearSelectionOnClick(true)
								.OnGenerateRow(this, &SGdhBet::OnQueueGenerateRow)
								.HeaderRow(GetQueueHeaderRow())
							]
						]
						+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
							[
								SNew(STextBlock).Text_Raw(this, &SGdhBet::GetQueueSummaryTxt)
							]
						]
					]
				]
			]
		]
	];
	// clang-format on
}

void SGdhBet::OnQueueRefresh() {
	QueueUpdateData();
	QueueUpdateView();
}

void SGdhBet::OnQueueProcess() {

	if (!GEditor) return;
	if (!Settings) return;
	// if (!Settings->World.LoadSynchronous()) return;
	// if (!Settings->RenderSettings.LoadSynchronous()) return;

	UMoviePipelineQueue* Queue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!Queue) return;

	for (const auto& Job : Queue->GetJobs()) {
		Queue->DeleteJob(Job);
	}

	// UMoviePipelineOutputSetting* OutputSetting =
	// 	Cast<UMoviePipelineOutputSetting>(Settings->RenderSettings->FindOrAddSettingByClass(
	// 		UMoviePipelineOutputSetting::StaticClass()
	// 	));
	// if (!OutputSetting) return;

	// OutputSetting->FileNameFormat = TEXT("{sequence_name}_{frame_number_rel}");
	// OutputSetting->bOverrideExistingOutput = true;
	// OutputSetting->ZeroPadFrameNumbers = 4;
	// OutputSetting->FrameNumberOffset = 0;
	// OutputSetting->HandleFrameCount = 0;
	// OutputSetting->OutputFrameStep = 1;
	// OutputSetting->bUseCustomPlaybackRange = false;
	// OutputSetting->CustomStartFrame = GetLevelSequenceStartFrame(InLevelSequence,
	// Settings->Framerate); OutputSetting->CustomEndFrame =
	// GetLevelSequenceEndFrame(InLevelSequence, Settings->Framerate);

	// const FString DirOutput = FPaths::ConvertRelativePathToFull(Settings->DirOutput.Path);
	//
	// EncodeCmds.Reset();
	//
	// for (const auto& Item : QueueItems) {
	// 	if (Item->bDragRow) continue;
	//
	// 	const auto Job = Queue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
	// 	Job->Map = Settings->World.ToSoftObjectPath();
	// 	Job->SetSequence(Item->LevelSequence);
	//
	// 	// {output}/{list}/images/{sequence}/{sequence}.*png
	// 	const FString NameList = Item->NameRenderList;
	// 	const FString NameSequence = Item->NameSequence;
	// 	const FString DirOutputImages =
	// 		FString::Printf(TEXT("%s/%s/images/%s"), *DirOutput, *NameList, *NameSequence);
	//
	// 	if (FPaths::DirectoryExists(*DirOutputImages)) {
	// 		FPlatformFileManager::Get().GetPlatformFile().DeleteDirectory(*DirOutputImages);
	// 	}
	// 	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*DirOutputImages);
	//
	// 	OutputSetting->OutputDirectory.Path = DirOutputImages;
	//
	// 	Job->SetConfiguration(Settings->RenderSettings.Get());

	// ENCODING
	// for (const auto& Preset : Settings->Pipelines) {
	// 	const FString PresetName = Preset->PresetName.ToString();
	// 	const FString EncodeCmd =
	// 		UKismetStringLibrary::JoinStringArray(Preset->EncodeCmd, TEXT(" "));
	// 	const FString TokenSeqPath =
	// 		FString::Printf(TEXT("\"%s/%s.%%04d.png\""), *DirOutputImages, *NameSequence);
	//
	// 	// {output}/{list}/videos/{preset}/{sequence}.mp4
	// 	const FString DirOutputVideo =
	// 		FString::Printf(TEXT("%s/%s/videos/%s"), *DirOutput, *NameList, *PresetName);
	//
	// 	if (FPaths::DirectoryExists(*DirOutputVideo)) {
	// 		FPlatformFileManager::Get().GetPlatformFile().DeleteDirectory(*DirOutputVideo);
	// 	}
	// 	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*DirOutputVideo);
	//
	// 	const FString TokenVideoPath = DirOutputVideo / NameSequence;
	// 	const FString FinalCommand = EncodeCmd.Replace(TEXT("{seq_path}"), *TokenSeqPath)
	// 									 .Replace(TEXT("{video_path}"), *TokenVideoPath);
	//
	// 	EncodeCmds.Add(FinalCommand);
	// }
	// }
	//
	// const auto Executor = Cast<UMoviePipelinePIEExecutor>(
	// 	GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(
	// 		UMoviePipelinePIEExecutor::StaticClass()
	// 	)
	// );
	// if (!Executor) return;
	//
	// Executor->OnExecutorFinished().AddRaw(this, &SGdhBet::OnRenderFinished);
}

void SGdhBet::QueueUpdateData() {
	QueueItems.Reset();
}

void SGdhBet::QueueUpdateView() {
	if (!QueueView) return;

	QueueView->RebuildList();
}

void SGdhBet::OnRenderFinished(UMoviePipelineExecutorBase* Executor, bool bSuccess) {
	if (!bSuccess) return;

	for (const auto& Cmd : EncodeCmds) {

		uint32 ProcessId;
		FProcHandle ProcessHandle =
			FPlatformProcess::CreateProc(*Settings->FFmpegExePath.FilePath, *(TEXT(" ") + Cmd), true, false, false, &ProcessId, 0, nullptr, nullptr);

		if (ProcessHandle.IsValid()) {
			FPlatformProcess::WaitForProc(ProcessHandle);
		}

		// FPlatformProcess::CloseProc(ProcessHandle);
	}
}

void SGdhBet::OnRenderListDblClick(const FAssetData& AssetData) {
	UGdhLibEditor::OpenAssetEditor(AssetData);
}

void SGdhBet::OnEncodePresetDblClick(const FAssetData& AssetData) {
	UGdhLibEditor::OpenAssetEditor(AssetData);
}

FReply SGdhBet::OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return FReply::Unhandled();
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return FReply::Unhandled();

	const auto AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	FScopedTransaction Transaction {FText::FromName(TEXT("GdhBetDragDropOperation"))};

	const auto DraggedAssets = AssetDragDropOp->GetAssets();

	for (const auto& Asset : DraggedAssets) {
		const UGdhBetRenderList* RenderList = Cast<UGdhBetRenderList>(Asset.GetAsset());
		if (!RenderList) continue;

		const FString NameRenderList = RenderList->GetName();

		{
			UGdhBetQueueItem* NewItem = NewObject<UGdhBetQueueItem>();
			if (!NewItem) continue;

			NewItem->NameRenderList = NameRenderList;
			NewItem->bDragRow = true;

			QueueItems.Add(NewItem);
		}

		for (const auto& Seq : RenderList->Sequences) {
			if (!Seq.LoadSynchronous()) continue;

			const FString NameSeq = Seq->GetName();

			UGdhBetQueueItem* NewItem = NewObject<UGdhBetQueueItem>();
			if (!NewItem) continue;

			NewItem->bDragRow = false;
			NewItem->NameRenderList = NameRenderList;
			NewItem->NameSequence = NameSeq;
			NewItem->LevelSequence = Seq.Get();

			QueueItems.Add(NewItem);
		}
	}

	QueueUpdateView();

	return FReply::Handled();
}

bool SGdhBet::CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return false;
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return false;

	const auto AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	for (const auto& Asset : AssetDragDropOp->GetAssets()) {
		if (Cast<UGdhBetRenderList>(Asset.GetAsset())) {
			return true;
		}
	}

	return false;
}

FText SGdhBet::GetQueueSummaryTxt() const {
	// TODO:ashe23 not queue items num, but sequences num
	return FText::FromString(FString::Printf(TEXT("Total: %d"), QueueItems.Num()));
}

TSharedRef<SHeaderRow> SGdhBet::GetQueueHeaderRow() {
	// clang-format off
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(TEXT("RenderList"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("List")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("SeqName"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Sequence")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Presets"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Presets")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Tracks"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("AudioTracks")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];
	// clang-format on
}

TSharedRef<ITableRow> SGdhBet::OnQueueGenerateRow(TWeakObjectPtr<UGdhBetQueueItem> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGdhBetQueueItem, OwnerTable).QueueItem(Item);
}

TSharedRef<SWidget> SGdhBet::CreateToolbarQueue() const {
	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};

	ToolBarBuilder.BeginSection("GdhBetRenderListMainToolbar");
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRefresh);
	ToolBarBuilder.AddSeparator();
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetProcess);
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}
