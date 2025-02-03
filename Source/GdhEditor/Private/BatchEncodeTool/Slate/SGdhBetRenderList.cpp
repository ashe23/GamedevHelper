// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBetRenderList.h"
#include "BatchEncodeTool/Slate/SGdhBetRenderListItem.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "GdhLibAsset.h"
#include "GdhLibEditor.h"
#include "LevelSequence.h"
#include "SDropTarget.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhBetRenderList::Construct(const FArguments& InArgs) {
	if (!InArgs._RenderList) return;

	RenderList = InArgs._RenderList;

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCmds::Get().BetRenderListRefresh,
		FExecuteAction::CreateRaw(this, &SGdhBetRenderList::OnListRefresh)
	);
	Cmds->MapAction(
		FGdhCmds::Get().BetRenderListRemove,
		FExecuteAction::CreateRaw(this, &SGdhBetRenderList::OnListRemove)
	);

	ListUpdateData();
	ListUpdateView();

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
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhBetRenderListItem>>)
						.ListItemsSource(&ListItems)
						.SelectionMode(ESelectionMode::Multi)
						.ClearSelectionOnClick(true)
						.OnGenerateRow(this, &SGdhBetRenderList::OnGenerateRow)
						.OnMouseButtonDoubleClick_Raw(this, &SGdhBetRenderList::OnListDblClick)
						.HeaderRow(GetHeaderRow())
					]
				]
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
	];
	// clang-format on
}

void SGdhBetRenderList::ListUpdateData() {
	ListItems.Reset(RenderList->Sequences.Num());

	for (const auto& Seq : RenderList->Sequences) {

		const ULevelSequence* Sequence = Seq.LoadSynchronous();
		if (!Sequence) continue;

		UGdhBetRenderListItem* NewItem = NewObject<UGdhBetRenderListItem>();
		if (!NewItem) continue;

		const FFrameRate FrameRate = UGdhLibAsset::GetLevelSequenceFrameRate(Sequence);
		const int32 FrameStart = UGdhLibAsset::GetLevelSequenceStartFrame(Sequence, FrameRate);
		const int32 FrameEnd = UGdhLibAsset::GetLevelSequenceEndFrame(Sequence, FrameRate);
		const int32 DurFrames = UGdhLibAsset::GetLevelSequenceDurationInFrames(Sequence, FrameRate);
		const float DurSec = UGdhLibAsset::GetLevelSequenceDurationInSeconds(Sequence, FrameRate);
		const bool bHasSlomoTrack = UGdhLibAsset::LevelSequenceHasSlomoTrack(Sequence);

		NewItem->Name = Sequence->GetName();
		NewItem->FrameStart = FString::FromInt(FrameStart);
		NewItem->FrameEnd = FString::FromInt(FrameEnd);
		NewItem->FrameRate = FString::Printf(TEXT("%s"), *FrameRate.ToPrettyText().ToString());
		NewItem->DurationFrames = FString::FromInt(DurFrames);
		NewItem->DurationHuman = FString::Printf(TEXT("%.2f sec"), DurSec);
		NewItem->HasTrackSlomo = bHasSlomoTrack ? TEXT("Yes") : TEXT("No");
		NewItem->Sequence = Sequence;

		ListItems.Add(NewItem);
	}
}

void SGdhBetRenderList::ListUpdateView() {
	if (!ListView) return;

	ListView->RebuildList();
}

void SGdhBetRenderList::OnListRefresh() {
	ListUpdateData();
	ListUpdateView();
}

void SGdhBetRenderList::OnListRemove() {

	const auto& SelectedItems = ListView->GetSelectedItems();

	for (const auto& Item : SelectedItems) {
		if (RenderList->Sequences.Contains(Item->Sequence)) {
			RenderList->Sequences.Remove(Item->Sequence);
		}
	}

	RenderList->Modify();

	ListUpdateData();
	ListUpdateView();
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

	RenderList->Sequences.Reserve(RenderList->Sequences.Num() + DraggedAssets.Num());
	for (const auto& Asset : AssetDragDropOp->GetAssets()) {

		const ULevelSequence* Sequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (!Sequence) continue;
		if (RenderList->Sequences.Contains(Sequence)) continue;

		RenderList->Sequences.Add(Sequence);
	}

	RenderList->Modify();

	ListUpdateData();
	ListUpdateView();

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
	const int32 Selected = ListView->GetSelectedItems().Num();
	const int32 Total = RenderList->Sequences.Num();

	if (Selected > 0) {
		return FText::FromString(
			FString::Printf(TEXT("Total: %d - (Selected %d) sequences"), Total, Selected)
		);
	}

	return FText::FromString(FString::Printf(TEXT("Total: %d sequences"), Total));
}

TSharedRef<SWidget> SGdhBetRenderList::CreateToolbarMain() const {
	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};

	ToolBarBuilder.BeginSection("GdhBetRenderListMainToolbar");
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRenderListRefresh);
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetRenderListRemove);
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
	   .HeaderContentPadding(FMargin{5.0f})
	   [
		   SNew(STextBlock)
		   .Text(FText::FromString(TEXT("Duration In Sec")))
		   .ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
		   .Font(FGdhStyles::GetFont("Light", 10.0f))
	   ]
		+ SHeaderRow::Column(TEXT("TrackSlomo"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Has Slomo Track")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];

	// clang-format on
}

TSharedRef<ITableRow> SGdhBetRenderList::OnGenerateRow(
	TWeakObjectPtr<UGdhBetRenderListItem> Item, const TSharedRef<STableViewBase>& OwnerTable
) {
	return SNew(SGdhBetRenderListItem, OwnerTable).ListItem(Item);
}
