// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerListItem.h"

#include "GdhStyles.h"

void SGdhRenderingManagerListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderingManagerListItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderingManagerListItem>>::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhRenderingManagerListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	// todo:ashe23 check for rendered images
	
	if (InColumnName.IsEqual(TEXT("Status")))
	{
		const FString Icon = FGdhStyles::GetIconByStatus(EGdhGenericStatus::OK);
		return SNew(SBox)
			.WidthOverride(20)
			.HeightOverride(20)
		[
			SNew(SImage)
			.Image(FGdhStyles::GetIcon(Icon))
		];
	}

	if (InColumnName.IsEqual(TEXT("Sequence")))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->LevelSequence->GetName()));
	}

	if (InColumnName.IsEqual(TEXT("Note")))
	{
		return SNew(STextBlock).Text(FText::FromString("Some note"));
	}
	
	return SNew(STextBlock).Text(FText::FromString(""));
}
