// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/Slate/SGdhAssetNamingToolListItem.h"

#include "GdhConstants.h"
#include "GdhLibAsset.h"
#include "GdhLibEditor.h"
#include "GdhLibString.h"
#include "GdhStructs.h"
#include "GdhStyles.h"
#include "AssetNamingTool/GdhAssetNamingToolSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"

void SGdhAssetNamingToolListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);

	Settings = GetDefault<UGdhAssetNamingToolSettings>();
}

TSharedRef<SWidget> SGdhAssetNamingToolListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Status"))
	{
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().Padding(FMargin{2.0f, 0.0f}).AutoWidth().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SBox).WidthOverride(16).HeightOverride(16)
				[
					SAssignNew(StatusIcon, SImage).Image(FStyleDefaults::GetNoBrush())
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
		if (ListItem->bEditMode)
		{
			return
				SNew(SBox).Padding(FMargin{5.0f, 0.0f})
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
					[
						SNew(SBorder).HAlign(HAlign_Left)
						[
							SNew(SEditableText)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString(ListItem->OldName))
							.OnTextChanged(this, &SGdhAssetNamingToolListItem::OnAssetNameChanged)
						]
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

		return
			SNew(SBox).Padding(FMargin{5.0f, 0.0f})
			[
				SNew(SHorizontalBox)
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
						SAssignNew(NamePreview, STextBlock).Justification(ETextJustify::Center).Text(FText::FromString(ListItem->NewName))
					]
				]
			];
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}

void SGdhAssetNamingToolListItem::OnAssetNameChanged(const FText& Text) const
{
	if (!Settings) return;
	if (!StatusIcon.IsValid()) return;
	if (!NoteBlock.IsValid()) return;
	if (!NamePreview.IsValid()) return;

	ListItem->bDirty = true;

	const FGdhAssetNameAffix Affix = UGdhLibAsset::GetAssetNameAffix(ListItem->AssetData, Settings->Mappings.LoadSynchronous(), Settings->BlueprintTypes);
	const FString InputText = Text.ToString();
	const FString PreviewName = UGdhLibAsset::GetAssetNameByConvention(
		InputText,
		Affix,
		Settings->AssetNamingCase,
		Settings->PrefixNamingCase,
		Settings->SuffixNamingCase,
		Settings->Delimiter
	);

	NamePreview->SetText(FText::FromString(PreviewName));

	const FString AssetPreviewObjectPath = ListItem->AssetData.PackagePath.ToString() + FString::Printf(TEXT("/%s.%s"), *PreviewName, *PreviewName);
	const FAssetData ExistingAssetData = UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetByObjectPath(FName{*AssetPreviewObjectPath});

	// todo:ashe23 how to check multiple edited name conflicts?
	
	if (InputText.IsEmpty())
	{
		StatusIcon->SetImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Warning")));
		NoteBlock->SetText(FText::FromString(TEXT("Asset name cant be empty")));
	}
	else if (!UGdhLibString::HasOnly(InputText, GdhConstants::ValidAssetNameChars))
	{
		StatusIcon->SetImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Warning")));
		NoteBlock->SetText(FText::FromString(TEXT("Asset name has invalid characters")));
	}
	else if (ExistingAssetData.IsValid())
	{
		StatusIcon->SetImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Warning")));
		NoteBlock->SetText(FText::FromString(TEXT("Asset with same name already exists in current path")));
	}
	else if (!InputText.Equals(ListItem->AssetData.AssetName.ToString(), ESearchCase::CaseSensitive))
	{
		StatusIcon->SetImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Modified")));
		NoteBlock->SetText(FText{});
		ListItem->NewName = PreviewName;
	}
	else
	{
		StatusIcon->SetImage(FStyleDefaults::GetNoBrush());
		NoteBlock->SetText(FText{});
		ListItem->NewName = PreviewName;
	}
}
