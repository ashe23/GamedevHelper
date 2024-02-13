// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/Slate/SGdhAssetNamingToolListItem.h"
#include "GdhStyles.h"
#include "Widgets/Input/SHyperlink.h"

void SGdhAssetNamingToolListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhAssetNamingToolListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Preview"))
	{
		const TSharedPtr<FAssetThumbnail> AssetThumbnail = MakeShareable(new FAssetThumbnail(ListItem->AssetData, 16, 16, nullptr));
		const FAssetThumbnailConfig ThumbnailConfig;

		return
			SNew(SBox)
			.WidthOverride(16)
			.HeightOverride(16)
			[
				AssetThumbnail->MakeThumbnailWidget(ThumbnailConfig)
			];
	}

	if (InColumnName == TEXT("AssetClass"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->AssetData.AssetClass.ToString()));
	}

	if (InColumnName == TEXT("Result"))
	{
		return
			SNew(SBox)
			.Padding(FMargin{5.0f, 0.0f})
			[

				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Bg")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red")))
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(ListItem->OldName))
						.HighlightColor(FLinearColor{FColor::Orange})
						.HighlightText(FText::FromString(TEXT("basic"))) // todo:ashe23 highlight settings as parameter
					]
				]
				+ SHorizontalBox::Slot()
				  .Padding(FMargin{5.0f, 0.0f})
				  .AutoWidth()
				[
					SNew(SImage)
					.Image(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Arrow")))
					.ColorAndOpacity(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Grey")))
				]
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Bg")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Green")))
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(ListItem->NewName))
					]
				]
			];
	}

	if (InColumnName == TEXT("Path"))
	{
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(FMargin{5.0f, 0.0f})
			[
				SNew(SHyperlink)
				.Text(FText::FromString(ListItem->AssetData.PackagePath.ToString()))
				.OnNavigate_Lambda([&]()
				                {
					                // UGdhLibEditor::OpenAssetInContentBrowser(ListItem->AssetData);
				                })
			];
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
