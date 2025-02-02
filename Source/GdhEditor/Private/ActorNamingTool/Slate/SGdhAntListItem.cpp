// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ActorNamingTool/Slate/SGdhAntListItem.h"

#include "GdhStyles.h"

void SGdhAntListItem::Construct(
	const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView
) {
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments().Padding(FMargin {0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}
TSharedRef<SWidget> SGdhAntListItem::GenerateWidgetForColumn(const FName& InColumnName) {

	// clang-format off
	if (InColumnName == TEXT("Preview"))
	{
		return
			SNew(SBox).Padding(FMargin{5.0f, 0.0f})
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center).Padding(FMargin{5.0f, 0.0f})
				[
					SNew(SBox).WidthOverride(16).HeightOverride(16)
					[
						SNew(SImage).Image(ListItem->ActorIcon)
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
				[
					SNew(STextBlock).Justification(ETextJustify::Center).Text(FText::FromString(ListItem->OldName))
				]
				+ SHorizontalBox::Slot().Padding(FMargin{5.0f, 0.0f}).AutoWidth()
				[
					SNew(SImage)
					.Image(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Arrow")))
					.ColorAndOpacity(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Grey")))
				]
				+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Bg")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Green")))
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock).Justification(ETextJustify::Center).Text(FText::FromString(ListItem->NewName))
					]
				]
			];
	}

	if (InColumnName == TEXT("Prefix"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->Prefix)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("Suffix"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->Suffix)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FolderName"))
	{
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth().Padding(FMargin{10.0f, 0.0f, 5.0f, 0.0f})
			[
				SNew(SBox).WidthOverride(16).HeightOverride(16)
				[
					SNew(SImage).Image(FEditorStyle::Get().GetBrush("SceneOutliner.FolderOpen"))
				]
			]
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).FillWidth(1.0f).Padding(FMargin{5.0f, 0.0f, 0.0f, 0.0f})
			[
				SNew(STextBlock).Text(FText::FromString(ListItem->FolderName)).Justification(ETextJustify::Left)
			];
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));

	// clang-format on
}
