// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ActorNamingTool/Slate/SGdhActorNamingToolListItem.h"

void SGdhActorNamingToolListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhActorNamingToolListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Prefix"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->Prefix)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("Suffix"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->Suffix)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("OldName"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->OldName)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("NewName"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->NewName)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FolderName"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->FolderName)).Justification(ETextJustify::Center);
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
