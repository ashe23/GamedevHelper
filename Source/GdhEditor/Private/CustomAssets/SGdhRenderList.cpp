// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/SGdhRenderList.h"

#include "GdhLibAsset.h"
#include "CustomAssets/GdhRenderList.h"
#include "CustomAssets/SGdhRenderListItem.h"
#include "GdhStyles.h"
#include "SDropTarget.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhRenderList::Construct(const FArguments& InArgs) {

	if (!InArgs._RenderList) return;

	RenderList = InArgs._RenderList;

	ListUpdateData();
	ListUpdateView();

	// clang-format off
	ChildSlot
	[
		SNew(SDropTarget)
		.OnDrop(this, &SGdhRenderList::OnDragDropTarget)
		.OnAllowDrop(this, &SGdhRenderList::CanDragDropTarget)
		.OnIsRecognized(this, &SGdhRenderList::CanDragDropTarget)
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
					SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhRenderListItem>>)
					.ListItemsSource(&ListItems)
					.SelectionMode(ESelectionMode::None)
					.OnGenerateRow(this, &SGdhRenderList::OnGenerateRow)
					.HeaderRow(GetHeaderRow())
				]
			]
		]
	];
	// clang-format on
}

void SGdhRenderList::ListUpdateData() {

	ListItems.Reset(RenderList->Sequences.Num());

	for (const auto& Seq : RenderList->Sequences) {

		const ULevelSequence* Sequence = Seq.LoadSynchronous();
		if (!Sequence) continue;

		UGdhRenderListItem* NewItem = NewObject<UGdhRenderListItem>();
		if (!NewItem) continue;

		const FFrameRate FrameRate = UGdhLibAsset::GetLevelSequenceFrameRate(Sequence);
		const int32 FrameStart = UGdhLibAsset::GetLevelSequenceStartFrame(Sequence, FrameRate);
		const int32 FrameEnd = UGdhLibAsset::GetLevelSequenceEndFrame(Sequence, FrameRate);
		const int32 DurationFrames = UGdhLibAsset::GetLevelSequenceDurationInFrames(Sequence, FrameRate);
		const float DurationSec = UGdhLibAsset::GetLevelSequenceDurationInSeconds(Sequence, FrameRate);

		NewItem->Name = Sequence->GetName();
		NewItem->FrameStart = FString::FromInt(FrameStart);
		NewItem->FrameEnd = FString::FromInt(FrameEnd);
		NewItem->FrameRate = FString::Printf(TEXT("%s"), *FrameRate.ToPrettyText().ToString());
		NewItem->Duration = FString::Printf(TEXT("%d frames (~ %.2f sec )"), DurationFrames, DurationSec);

		ListItems.Add(NewItem);
	}
}

void SGdhRenderList::ListUpdateView() {
	if (!ListView) return;

	ListView->RebuildList();
}

FReply SGdhRenderList::OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return FReply::Unhandled();
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return FReply::Unhandled();

	const TSharedPtr<FAssetDragDropOp> AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	FScopedTransaction Transaction {FText::FromName(TEXT("GdhDragDropOperation"))};

	const auto DraggedAssets = AssetDragDropOp->GetAssets();

	RenderList->Sequences.Reserve(RenderList->Sequences.Num() + DraggedAssets.Num());
	for (const auto& Asset : AssetDragDropOp->GetAssets()) {

		const ULevelSequence* Sequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (!Sequence) continue;

		RenderList->Sequences.Add(Sequence);
	}

	RenderList->Modify();

	ListUpdateData();
	ListUpdateView();

	return FReply::Handled();
}

bool SGdhRenderList::CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return false;
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return false;

	TSharedPtr<FAssetDragDropOp> AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	for (const auto& Asset : AssetDragDropOp->GetAssets()) {
		ULevelSequence* Sequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (Sequence) {
			return true;
		}
	}

	return false;
}

TSharedRef<SHeaderRow> SGdhRenderList::GetHeaderRow() {

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
		+ SHeaderRow::Column(TEXT("Duration"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Duration")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];

	// clang-format on
}

TSharedRef<ITableRow> SGdhRenderList::OnGenerateRow(TWeakObjectPtr<UGdhRenderListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGdhRenderListItem, OwnerTable).ListItem(Item);
}
