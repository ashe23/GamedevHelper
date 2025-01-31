// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "VideoEncoderTool/Slate/SGdhVideoEncoderToolListItem.h"

void SGdhVideoEncoderToolListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView) {
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(SMultiColumnTableRow::FArguments().Padding(FMargin {0.0f, 2.0f, 0.0f, 0.0f}), InOwnerTableView);
}

TSharedRef<SWidget> SGdhVideoEncoderToolListItem::GenerateWidgetForColumn(const FName& InColumnName) {

	if (InColumnName == TEXT("NameQueue")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->NameQueue)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("NameSequence")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->NameSequence)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FrameStart")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->FrameStart)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FrameEnd")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->FrameEnd)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FrameRate")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->FrameRate)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("Duration")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->Duration)).Justification(ETextJustify::Center);
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
