// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/Slate/SGdhAssetNamingToolListItem.h"
#include "GdhLibAsset.h"
#include "GdhStyles.h"

void SGdhAssetNamingToolListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;
	DelegateRowAssetNameChanged = InArgs._OnAssetNameChange;

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhAssetNamingToolListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Status"))
	{
		const auto Image = ListItem->bHasErrors ? FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Warning")) : FStyleDefaults::GetNoBrush();
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().Padding(FMargin{2.0f, 0.0f}).AutoWidth().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SBox).WidthOverride(16).HeightOverride(16)
				[
					SAssignNew(StatusIcon, SImage).Image(Image)
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

	if (InColumnName == TEXT("AssetClass"))
	{
		const TSharedPtr<FAssetThumbnail> AssetThumbnail = MakeShareable(new FAssetThumbnail(ListItem->AssetData, 16, 16, nullptr));
		FAssetThumbnailConfig ThumbnailConfig;
		ThumbnailConfig.bForceGenericThumbnail = true;

		const EGdhBlueprintType BlueprintType = UGdhLibAsset::GetBlueprintType(ListItem->AssetData);
		const FName AssetExactClassName = UGdhLibAsset::GetAssetExactClassName(ListItem->AssetData);
		const FString Str = BlueprintType == EGdhBlueprintType::None ? AssetExactClassName.ToString() : FString::Printf(TEXT("Blueprint ( %s )"), *AssetExactClassName.ToString());

		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth().Padding(FMargin{10.0f, 0.0f, 5.0f, 0.0f})
			[
				SNew(SBox).WidthOverride(16).HeightOverride(16)
				[
					AssetThumbnail->MakeThumbnailWidget(ThumbnailConfig)
				]
			]
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).FillWidth(1.0f).Padding(FMargin{5.0f, 0.0f, 0.0f, 0.0f})
			[
				SNew(STextBlock).Text(FText::FromString(Str)).Justification(ETextJustify::Left)
			];
	}

	if (InColumnName == TEXT("Result"))
	{
		TSharedPtr<SWidget> NameBlockWidget;
		if (ListItem->bEditMode)
		{
			NameBlockWidget = SNew(SBorder).HAlign(HAlign_Left)
			[
				SNew(SEditableText)
				.Justification(ETextJustify::Center)
				.Text(FText::FromString(ListItem->OldName))
				.OnTextChanged(this, &SGdhAssetNamingToolListItem::OnItemNameChanged)
			];
		}
		else
		{
			NameBlockWidget = SNew(STextBlock).Justification(ETextJustify::Center).Text(FText::FromString(ListItem->OldName));
		}

		return
			SNew(SBox).Padding(FMargin{5.0f, 0.0f})
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
				[
					NameBlockWidget.ToSharedRef()
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
						SAssignNew(NamePreview, STextBlock).Justification(ETextJustify::Center).Text(FText::FromString(ListItem->NewName))
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center).Padding(FMargin{10.0f, 0.0f})
				[
					SAssignNew(NoteBlock, STextBlock)
					.Text(FText::FromString(ListItem->Note))
					.ColorAndOpacity(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red")))
				]
			];
	}

	if (InColumnName == TEXT("Path"))
	{
		return SNew(STextBlock).Text(FText::FromName(ListItem->AssetData.PackagePath)).Justification(ETextJustify::Left);
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}

void SGdhAssetNamingToolListItem::OnItemNameChanged(const FText& Text) const
{
	if (DelegateRowAssetNameChanged.IsBound())
	{
		DelegateRowAssetNameChanged.Execute(ListItem, Text.ToString());
	}

	if (!StatusIcon.IsValid()) return;
	if (!NoteBlock.IsValid()) return;
	if (!NamePreview.IsValid()) return;

	if (ListItem->bDirty)
	{
		StatusIcon->SetImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Modified")));
	}

	if (ListItem->bHasErrors)
	{
		StatusIcon->SetImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Warning")));
	}

	NoteBlock->SetText(FText::FromString(ListItem->Note));
	NamePreview->SetText(FText::FromString(ListItem->NewName));
}
