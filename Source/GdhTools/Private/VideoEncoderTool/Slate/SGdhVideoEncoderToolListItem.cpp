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

	if (InColumnName == TEXT("Preview")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->EncodeCmdPreview)).Justification(ETextJustify::Center);
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
