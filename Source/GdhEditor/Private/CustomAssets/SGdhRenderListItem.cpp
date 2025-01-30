// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/SGdhRenderListItem.h"

void SGdhRenderListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView) {
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(SMultiColumnTableRow::FArguments().Padding(FMargin {0.0f, 2.0f, 0.0f, 0.0f}), InOwnerTableView);
}

TSharedRef<SWidget> SGdhRenderListItem::GenerateWidgetForColumn(const FName& InColumnName) {

	if (InColumnName == TEXT("Name")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->Name)).Justification(ETextJustify::Center);
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
