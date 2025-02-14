// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBetQueueItem.h"
#include "SDropTarget.h"
#include "DragAndDrop/AssetDragDropOp.h"

void SGdhBetQueueItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView) {
	QueueItem = InArgs._QueueItem;

	SMultiColumnTableRow::Construct(SMultiColumnTableRow::FArguments().Padding(FMargin {0.0f, 2.0f, 0.0f, 0.0f}), InOwnerTableView);
}

TSharedRef<SWidget> SGdhBetQueueItem::GenerateWidgetForColumn(const FName& InColumnName) {

	if (InColumnName == TEXT("RenderList") && QueueItem->bDragRow) {
		return SNew(STextBlock).Text(FText::FromString(QueueItem->NameRenderList)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("SeqName") && !QueueItem->bDragRow) {
		return SNew(STextBlock).Text(FText::FromString(QueueItem->NameSequence)).Justification(ETextJustify::Center);
	}

	// if (QueueItem->bDragRow) {
	//
	// 	if (InColumnName == TEXT("Presets")) {
	//
	// 		// clang-format off
	// 		return
	// 			SNew(SDropTarget)
	// 			.OnDrop(this, &SGdhBetQueueItem::OnDragDropTarget)
	// 			.OnAllowDrop(this, &SGdhBetQueueItem::CanDragDropTarget)
	// 			.OnIsRecognized(this, &SGdhBetQueueItem::CanDragDropTarget)
	// 			[
	// 				SNew(STextBlock).Text(FText::FromString(TEXT("Mp4 HLS")))
	// 			];
	// 		// clang-format on
	// 	}
	//
	// 	if (InColumnName == TEXT("Tracks")) {
	//
	// 		// clang-format off
	// 		return
	// 			SNew(SDropTarget)
	// 			[
	// 				SNew(STextBlock).Text(FText::FromString(TEXT("track_01 track_02")))
	// 			];
	// 		// clang-format on
	// 	}
	// }

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}

FReply SGdhBetQueueItem::OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return FReply::Unhandled();
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return FReply::Unhandled();

	const auto AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	FScopedTransaction Transaction {FText::FromName(TEXT("GdhBetDragDropOperation"))};

	const auto DraggedAssets = AssetDragDropOp->GetAssets();

	// TODO:ashe23 drag and drop functionality later

	return FReply::Handled();
}

bool SGdhBetQueueItem::CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation) {
	if (!InOperation) return false;
	if (!InOperation->IsOfType<FAssetDragDropOp>()) return false;

	// const auto AssetDragDropOp = StaticCastSharedPtr<FAssetDragDropOp>(InOperation);
	// for (const auto& Asset : AssetDragDropOp->GetAssets()) {
	// 	if (Cast<UGdhBetEncodePreset>(Asset.GetAsset())) {
	// 		return true;
	// 	}
	// }

	return false;
}
