// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBetRenderList.h"
#include "BatchEncodeTool/Slate/SGdhBetRenderListItem.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "GdhLibAsset.h"
#include "GdhLibEditor.h"
#include "IContentBrowserSingleton.h"
#include "LevelSequence.h"
#include "SDropTarget.h"
#include "DragAndDrop/AssetDragDropOp.h"
// #include "Kismet/KismetStringLibrary.h"
#include "GdhLibEncoder.h"
#include "GdhPluginSettings.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "BatchEncodeTool/GdhBetSettings.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/ScopedSlowTask.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhBetRenderList::Construct(const FArguments& InArgs) {
	if (!InArgs._RenderList) return;

	RenderList = InArgs._RenderList;
	RenderList->OnRenderListChanged.AddRaw(this, &SGdhBetRenderList::ListUpdate);

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(FGdhCmds::Get().BetRenderListRefresh, FExecuteAction::CreateRaw(this, &SGdhBetRenderList::OnListRefresh));
	Cmds->MapAction(
		FGdhCmds::Get().BetRenderListRemove,
		FExecuteAction::CreateRaw(this, &SGdhBetRenderList::OnListRemove),
		FCanExecuteAction::CreateRaw(this, &SGdhBetRenderList::CanRemoveListItems)
	);
	Cmds->MapAction(FGdhCmds::Get().BetRenderListRemoveAll, FExecuteAction::CreateRaw(this, &SGdhBetRenderList::OnListRemoveAll));
	Cmds->MapAction(
		FGdhCmds::Get().BetRenderListRencode, FExecuteAction::CreateRaw(this, &SGdhBetRenderList::OnRencode)
		// FCanExecuteAction::CreateRaw(this, &SGdhBetRenderList::CanRemoveListItems)
	);

	FARFilter Filter;
	Filter.ClassNames.Add(ULevelSequence::StaticClass()->GetFName());

	FAssetPickerConfig PickerConfig;
	PickerConfig.Filter = Filter;
	PickerConfig.bAllowDragging = true;

	const auto SequencesBrowser = UGdhLibEditor::GetModuleContentBrowser().Get().CreateAssetPicker(PickerConfig);

	ListUpdate();

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhBetRenderListSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(RenderList);

	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
		[
			CreateToolbarMain()
		]
		+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
		[
			SNew(SSeparator).Thickness(3.0f)
		]
		+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
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
							.Text(FText::FromString(TEXT("Tokens")))
						]
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("{input_img} - full path to rendered image sequences")))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("{output_dir} - full path to output directory")))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("{input_audio:{track_name}} - full path to audio track name that specified in AudioTrack parameter, change {track_name} with your desired track")))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("{name_sequence} - name of level sequence currently processing")))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("{name_renderlist} - name of current render list asset")))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
					[
						SNew(SSeparator).Thickness(3.0f)
					]
					+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
					[
						SettingsProperty
					]
				]
			]
			+ SSplitter::Slot().Value(0.2f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
				[
					SequencesBrowser
				]
			]
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SDropTarget)
				.OnDrop(this, &SGdhBetRenderList::OnDragDropTarget)
				.OnAllowDrop(this, &SGdhBetRenderList::CanDragDropTarget)
				.OnIsRecognized(this, &SGdhBetRenderList::CanDragDropTarget)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
					[
						SNew(SScrollBox)
						.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
						.AnimateWheelScrolling(true)
						.ScrollBarVisibility(EVisibility::Visible)
						// .AllowOverscroll(EAllowOverscroll::No)
						+ SScrollBox::Slot()
						[
							SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhBetRenderListItem>>)
							.ListItemsSource(&ListItems)
							.SelectionMode(ESelectionMode::Multi)
							.ClearSelectionOnClick(true)
							.OnGenerateRow_Static(&SGdhBetRenderList::OnGenerateRow)
							.OnMouseButtonDoubleClick_Raw(this, &SGdhBetRenderList::OnListDblClick)
							.HeaderRow(GetHeaderRow())
						]
					]
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
						[
							SNew(STextBlock).Text_Raw(this, &SGdhBetRenderList::GetSummaryTxt)
						]
					]
				]
			]

		]
	];
	// clang-format on
}

void SGdhBetRenderList::ListUpdate() {
	ListUpdateData();
	ListUpdateView();
}

void SGdhBetRenderList::ListUpdateData() {

	const UGdhPluginSettings* PluginSettings = GetDefault<UGdhPluginSettings>();
	if (!PluginSettings) return;

	const FString MainOutputDir = FPaths::ConvertRelativePathToFull(RenderList->DirOutput.Path);
	const FString FFmpegExePath = FPaths::ConvertRelativePathToFull(PluginSettings->FFmpegExePath.FilePath);

	ListItems.Reset(RenderList->Sequences.Num() * RenderList->EncodePresets.Num());

	// token => path
	TMap<FString, FString> AudioTrackTokenMap;

	if (RenderList->AudioTracks.Num() > 0) {
		for (const auto& AudioTrack : RenderList->AudioTracks) {
			const FString AudioTrackName = AudioTrack.Key;
			const FString AudioTrackPath = FPaths::ConvertRelativePathToFull(AudioTrack.Value.FilePath);

			AudioTrackTokenMap.Add(FString::Printf(TEXT("{input_audio:%s}"), *AudioTrackName), *AudioTrackPath);
		}
	}

	for (const auto& Seq : RenderList->Sequences) {

		const ULevelSequence* Sequence = Seq.LoadSynchronous();
		if (!Sequence) continue;

		for (const auto& EncodePreset : RenderList->EncodePresets) {

			UGdhBetRenderListItem* NewItem = NewObject<UGdhBetRenderListItem>();
			if (!NewItem) continue;

			const FFrameRate FrameRate = UGdhLibAsset::GetLevelSequenceFrameRate(Sequence);
			const int32 FrameStart = UGdhLibAsset::GetLevelSequenceStartFrame(Sequence, FrameRate);
			const int32 FrameEnd = UGdhLibAsset::GetLevelSequenceEndFrame(Sequence, FrameRate);
			const int32 DurFrames = UGdhLibAsset::GetLevelSequenceDurationInFrames(Sequence, FrameRate);
			const float DurSec = UGdhLibAsset::GetLevelSequenceDurationInSeconds(Sequence, FrameRate);
			const bool bHasSlomoTrack = UGdhLibAsset::LevelSequenceHasSlomoTrack(Sequence);

			const FString EncodePresetName = EncodePreset.Key;

			FString EncodeCmd = UKismetStringLibrary::JoinStringArray(EncodePreset.Value.EncodeCmd, TEXT(" "));

			// {dir_output}/images/{renderlist}/{sequence_name}/{sequence_name}.%0{padding}d.{img_format}
			const FString TokenInputImg = FString::Printf(
				TEXT("\"%s/images/%s/%s/%s.%%0%dd.%s\""),
				*MainOutputDir,
				*RenderList->GetName(),
				*Sequence->GetName(),
				*Sequence->GetName(),
				UGdhLibEncoder::GetZeroPadding(RenderList->RenderSettings.LoadSynchronous()),
				*UGdhLibEncoder::GetImageExtension(RenderList->RenderSettings.LoadSynchronous())
			);

			// TODO:ashe23 finalize token list and show it in user interface for user
			EncodeCmd = EncodeCmd.Replace(TEXT("{input_img}"), *TokenInputImg);
			EncodeCmd = EncodeCmd.Replace(TEXT("{output_dir}"), *MainOutputDir);
			EncodeCmd = EncodeCmd.Replace(TEXT("{name_sequence}"), *Sequence->GetName());
			EncodeCmd = EncodeCmd.Replace(TEXT("{name_renderlist}"), *RenderList->GetName());

			for (const auto& Token : AudioTrackTokenMap) {
				if (EncodeCmd.Contains(Token.Key)) {
					EncodeCmd = EncodeCmd.Replace(*Token.Key, *Token.Value);
				}
			}

			const FString FinalCmd = FString::Printf(TEXT("%s %s"), *FFmpegExePath, *EncodeCmd);

			NewItem->Name = Sequence->GetName();
			NewItem->FrameStart = FString::FromInt(FrameStart);
			NewItem->FrameEnd = FString::FromInt(FrameEnd);
			NewItem->FrameRate = FString::Printf(TEXT("%s"), *FrameRate.ToPrettyText().ToString());
			NewItem->DurationFrames = FString::FromInt(DurFrames);
			NewItem->DurationHuman = FString::Printf(TEXT("%.2f sec"), DurSec);
			NewItem->HasTrackSlomo = bHasSlomoTrack ? TEXT("Yes") : TEXT("No");
			NewItem->EncodePresetName = EncodePresetName;
			NewItem->EncodeCmdPreview = FinalCmd;
			NewItem->Sequence = Sequence;

			ListItems.Add(NewItem);
		}
	}
}

void SGdhBetRenderList::ListUpdateView() const {
	if (!ListView) return;

	ListView->RebuildList();
}

void SGdhBetRenderList::OnListRefresh() {
	ListUpdate();
}

void SGdhBetRenderList::OnListRemove() {

	const auto& SelectedItems = ListView->GetSelectedItems();

	for (const auto& Item : SelectedItems) {
		if (RenderList->Sequences.Contains(Item->Sequence)) {
			RenderList->Sequences.Remove(Item->Sequence);
		}
	}

	RenderList->Modify();

	ListUpdate();
}

void SGdhBetRenderList::OnListRemoveAll() {
	RenderList->Sequences.Reset();
	RenderList->Modify();
	ListUpdate();
}

void SGdhBetRenderList::OnRencode() {
	ListUpdate();

	const UGdhPluginSettings* PluginSettings = GetDefault<UGdhPluginSettings>();
	if (!PluginSettings) return;

	if (PluginSettings->FFmpegExePath.FilePath.IsEmpty()) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("FFmpeg Exe Path not specified")));
		return;
	}

	if (RenderList->DirOutput.Path.IsEmpty()) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Output directory not specified")));
		return;
	}

	if (!FPaths::FileExists(PluginSettings->FFmpegExePath.FilePath)) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Invalid FFmpeg Exe Path")));
		return;
	}

	if (!FPaths::DirectoryExists(RenderList->DirOutput.Path)) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Invalid Output Directory")));
		return;
	}

	if (!RenderList->World.LoadSynchronous()) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to load World asset")));
		return;
	}

	if (!RenderList->RenderSettings.LoadSynchronous()) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to load RenderSettings")));
		return;
	}

	const FString MainOutputDir = FPaths::ConvertRelativePathToFull(RenderList->DirOutput.Path);
	const FString FFmpegExePath = FPaths::ConvertRelativePathToFull(PluginSettings->FFmpegExePath.FilePath);

	for (const auto& AudioTrack : RenderList->AudioTracks) {
		const FString AudioTrackName = AudioTrack.Key;
		const FString AudioTrackPath = FPaths::ConvertRelativePathToFull(AudioTrack.Value.FilePath);

		if (AudioTrackName.IsEmpty()) {
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Audio Track name cant be empty")));
			return;
		}

		if (!FPaths::FileExists(AudioTrackPath)) {
			const FString Msg = FString::Printf(TEXT("Invalid audio file specified for %s track"), *AudioTrackName);
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Msg));
			return;
		}
	}

	if (RenderList->Sequences.Num() == 0) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Specify Level Sequence assets in order to proceed")));
		return;
	}

	UMoviePipelineMasterConfig* RenderSettings = RenderList->RenderSettings.Get();

	UMoviePipelineQueue* Queue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!Queue) return;

	for (const auto& Job : Queue->GetJobs()) {
		Queue->DeleteJob(Job);
	}

	UMoviePipelineOutputSetting* OutputSetting = RenderSettings->FindSetting<UMoviePipelineOutputSetting>();
	if (!OutputSetting) return;

	EncodeCmds.Reset(ListItems.Num());

	for (const auto& Item : ListItems) {
		const auto Job = Queue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
		Job->Map = RenderList->World.LoadSynchronous();
		Job->SetSequence(Item->Sequence);

		// TODO:ashe23 move this into separate function?
		const FString DirOutput = FString::Printf(TEXT("\"%s/images/%s/%s/\""), *MainOutputDir, *RenderList->GetName(), *Item->Sequence->GetName());
		OutputSetting->OutputDirectory.Path = DirOutput;

		Job->SetConfiguration(RenderSettings);

		FString EncodeCmdInternal = Item->EncodeCmdPreview;
		EncodeCmdInternal.RemoveFromStart(FFmpegExePath);

		EncodeCmds.Add(EncodeCmdInternal);
	}

	OutputSetting->FileNameFormat = TEXT("{sequence_name}.{frame_number_rel}");
	OutputSetting->bOverrideExistingOutput = true;

	const auto Executor = Cast<UMoviePipelinePIEExecutor>(
		GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass())
	);
	if (!Executor) return;

	Executor->OnExecutorFinished().AddRaw(this, &SGdhBetRenderList::OnRencodeFinished);
}

void SGdhBetRenderList::OnRencodeFinished(UMoviePipelineExecutorBase*, bool bSuccess) {
	if (!bSuccess) return;

	const UGdhBetSettings* BetSettings = GetDefault<UGdhBetSettings>();
	if (!BetSettings) return;

	const FString FFmpegExePath = FPaths::ConvertRelativePathToFull(BetSettings->FFmpegExePath.FilePath);

	FScopedSlowTask SlowTask {static_cast<float>(EncodeCmds.Num()), FText::FromString(TEXT("Encoding ..."))};
	SlowTask.MakeDialog();

	for (const auto& Cmd : EncodeCmds) {
		SlowTask.EnterProgressFrame(1.0f);

		uint32 ProcessId;
		FProcHandle ProcessHandle =
			FPlatformProcess::CreateProc(*FFmpegExePath, *(TEXT(" ") + Cmd), true, false, false, &ProcessId, 0, nullptr, nullptr);

		if (ProcessHandle.IsValid()) {
			FPlatformProcess::WaitForProc(ProcessHandle);
		}
	}

	ListUpdate();
}

bool SGdhBetRenderList::CanRemoveListItems() {
	return ListView && (ListView->GetSelectedItems().Num() > 0);
}

void SGdhBetRenderList::OnListDblClick(TWeakObjectPtr<UGdhBetRenderListItem> Item) {
	if (!Item.IsValid()) return;

	UGdhLibEditor::OpenAssetEditor(Item->Sequence);
}

FReply SGdhBetRenderList::OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return FReply::Unhandled();
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return FReply::Unhandled();

	const auto AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	FScopedTransaction Transaction {FText::FromName(TEXT("GdhDragDropOperation"))};

	const auto DraggedAssets = AssetDragDropOp->GetAssets();
	bool bNotifyDuplicate = false;

	RenderList->Sequences.Reserve(RenderList->Sequences.Num() + DraggedAssets.Num());
	for (const auto& Asset : AssetDragDropOp->GetAssets()) {

		const ULevelSequence* Sequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (!Sequence) continue;
		if (RenderList->Sequences.Contains(Sequence)) {
			bNotifyDuplicate = true;
		}

		RenderList->Sequences.Add(Sequence);
	}

	RenderList->Modify();

	ListUpdate();

	if (bNotifyDuplicate) {
		const FString Msg = FString::Printf(TEXT("Some sequences already in the list"));
		UGdhLibEditor::ShowNotification(Msg, SNotificationItem::CS_Fail, 3.0f);
	}

	return FReply::Handled();
}

bool SGdhBetRenderList::CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return false;
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return false;

	const auto AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	for (const auto& Asset : AssetDragDropOp->GetAssets()) {
		const ULevelSequence* Sequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (Sequence) {
			return true;
		}
	}

	return false;
}

FText SGdhBetRenderList::GetSummaryTxt() const {
	const int32 NumSequences = RenderList->Sequences.Num();
	const int32 NumEncodePresets = RenderList->EncodePresets.Num();
	const int32 NumJobsTotal = ListItems.Num();
	const int32 NumJobsSelected = ListView->GetSelectedItems().Num();

	if (NumJobsSelected > 0) {
		return FText::FromString(
			FString::Printf(TEXT("Sequences: %d Jobs: %d Presets: %d Selected: %d"), NumSequences, NumJobsTotal, NumEncodePresets, NumJobsSelected)
		);
	}

	return FText::FromString(FString::Printf(TEXT("Sequences: %d Jobs: %d Presets: %d"), NumSequences, NumJobsTotal, NumEncodePresets));
}

TSharedRef<SWidget> SGdhBetRenderList::CreateToolbarMain() const {
	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};

	ToolBarBuilder.BeginSection("GdhBetRenderListMainToolbar");
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRenderListRefresh);
	ToolBarBuilder.AddSeparator();
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRenderListRemove);
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRenderListRemoveAll);
	ToolBarBuilder.AddSeparator();
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRenderListRencode);
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SHeaderRow> SGdhBetRenderList::GetHeaderRow() {
	// clang-format off
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(TEXT("Name"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Name")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("FrameStart"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.FixedWidth(100.0f)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("FrameStart")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("FrameEnd"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.FixedWidth(100.0f)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("FrameEnd")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("FrameRate"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.FixedWidth(100.0f)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("FrameRate")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("DurationFrames"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.FixedWidth(120.0f)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Duration In Frames")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+SHeaderRow::Column(TEXT("DurationHuman"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.FixedWidth(100.0f)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Duration In Sec")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+SHeaderRow::Column(TEXT("EncodePreset"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.FixedWidth(100.0f)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Encode Preset")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		// + SHeaderRow::Column(TEXT("TrackSlomo"))
		// .HAlignHeader(HAlign_Center)
		// .VAlignHeader(VAlign_Center)
		// .FixedWidth(100.0f)
		// .HeaderContentPadding(FMargin{5.0f})
		// [
		// 	SNew(STextBlock)
		// 	.Text(FText::FromString(TEXT("Has Slomo Track")))
		// 	.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
		// 	.Font(FGdhStyles::GetFont("Light", 10.0f))
		// ]
		+ SHeaderRow::Column(TEXT("EncodeCmdPreview"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		.FixedWidth(200.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Encode Cmd Preview")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];

	// clang-format on
}

TSharedRef<ITableRow> SGdhBetRenderList::OnGenerateRow(TWeakObjectPtr<UGdhBetRenderListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGdhBetRenderListItem, OwnerTable).ListItem(Item);
}
