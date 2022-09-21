// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerListItem.h"
#include "GdhStyles.h"
#include "Settings/GdhRenderingSettings.h"
#include "Libs/GdhRenderingLibrary.h"

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
	// checking for errors
	FString ErrorMsg;

	if (!ListItem.IsValid())
	{
		return SNew(STextBlock).Text(FText::FromString(""));
	}

	if (!ListItem->LevelSequence || !ListItem->Map)
	{
		return SNew(STextBlock).Text(FText::FromString(""));
	}

	// checking for warnings if no errors
	uint32 RenderedFramesNum = 0;

	if (ListItem->Status != EGdhGenericStatus::Error)
	{
		if (GEditor)
		{
			bool bHasMissingFrames = false;
			RenderedFramesNum = UGdhRenderingLibrary::GetRenderedFramesNum(ListItem->LevelSequence, bHasMissingFrames);

			if (RenderedFramesNum == 0)
			{
				ListItem->Status = EGdhGenericStatus::Warning;
				ErrorMsg = TEXT("Missing rendered images");
			}

			if (RenderedFramesNum > 0 && bHasMissingFrames)
			{
				ListItem->Status = EGdhGenericStatus::Warning;
				ErrorMsg = TEXT("Missing images for some frames. Image sequence is not sequential. Re-Render to fix this");
			}
		}
	}

	if (InColumnName.IsEqual(TEXT("Status")))
	{
		const FString Icon = FGdhStyles::GetIconByStatus(ListItem->Status);
		return SNew(SBox)
			.WidthOverride(20)
			.HeightOverride(20)
		[
			SNew(SImage)
			.Image(FGdhStyles::GetIcon(Icon))
		];
	}

	if (InColumnName.IsEqual(TEXT("Duration")))
	{
		const uint32 DurationInFrames = UGdhRenderingLibrary::GetLevelSequenceDuration(ListItem->LevelSequence);
		const double DurationInSeconds = DurationInFrames / GetDefault<UGdhRenderingSettings>()->Framerate.AsDecimal();
		const FString Duration = FString::Printf(TEXT("%d frames (%.1f sec)"), DurationInFrames, DurationInSeconds);
		
		return SNew(STextBlock).Text(FText::FromString(Duration));
	}

	if (InColumnName.IsEqual(TEXT("Sequence")))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->LevelSequence->GetName()));
	}

	if (InColumnName.IsEqual(TEXT("RenderedFrames")))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("%d"), RenderedFramesNum)));
	}

	if (InColumnName.IsEqual(TEXT("Note")))
	{
		const FLinearColor Color = FGdhStyles::GetColorByStatus(ListItem->Status);

		return SNew(STextBlock).ColorAndOpacity(Color).Text(FText::FromString(ErrorMsg));
	}

	return SNew(STextBlock).Text(FText::FromString(""));
}
