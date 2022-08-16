// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerListItem.h"
#include "UI/GamedevHelperEditorStyle.h"
// Engine Headers
#include "MoviePipelineQueueSubsystem.h"
#include "ProjectSettings/GamedevHelperRenderingSettings.h"
#include "Widgets/Input/SHyperlink.h"

void SGamedevHelperRenderingManagerListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._QueueItem;

	SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>>::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGamedevHelperRenderingManagerListItem::GenerateWidgetForColumn(const FName& InColumnName)
{

	const TSoftObjectPtr<UGamedevHelperRenderingSettings> RenderingSettings = GetDefault<UGamedevHelperRenderingSettings>();
	
	if (InColumnName == TEXT("State"))
	{
		const FString Icon = FGamedevHelperEditorStyle::GetIconByStatus(ListItem->Status);
		return
			SNew(SBox)
			.WidthOverride(20)
			.HeightOverride(20)
			[
				SNew(SImage)
				.Image(FGamedevHelperEditorStyle::GetIcon(Icon))
			];
	}

	if (InColumnName == TEXT("QueueName"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->QueueName));
	}

	if (InColumnName == TEXT("SequenceName"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->SequenceName));
	}

	if (InColumnName == TEXT("SequenceFrameStart"))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::FromInt(ListItem->SequenceStartFrame)));
	}

	if (InColumnName == TEXT("SequenceDuration"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->SequenceDuration));
	}

	if (InColumnName == TEXT("SequenceRenderedFrames"))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::FromInt(ListItem->SequenceRenderedFrames)));
	}

	if (InColumnName == TEXT("SequenceRenderedImagesDir"))
	{
		const FString Path = RenderingSettings->GetImageOutputDirectory(ListItem->QueueAsset.TryLoad(), ListItem->LevelSequence.TryLoad());

		if (!FPaths::DirectoryExists(Path))
		{
			return
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Render sequences in order this to be available")))
			.ColorAndOpacity(FLinearColor::Red)
			.Text(FText::FromString(TEXT("Not found")));
		}
		
		return SNew(SBox)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		[
			SNew(SHyperlink)
			.Text(FText::FromString(Path))
			.OnNavigate_Lambda([=]()
			{
				// ignore if we currently rendering sequences
				if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
				{
					return;
				}
				
				FPlatformProcess::ExploreFolder(*Path);
			})
		];
	}

	if (InColumnName == TEXT("SequenceRenderedVideoFile"))
	{
		const FString Path = RenderingSettings->GetVideoOutputDirectory(ListItem->QueueAsset.TryLoad());
		const FString FilePath = FString::Printf(TEXT("%s/%s.%s"), *Path, *ListItem->LevelSequence.GetAssetName(), *RenderingSettings->GetVideoExtension());

		if (!FPaths::FileExists(FilePath))
		{
			return
			SNew(STextBlock)
			.ToolTipText(FText::FromString(TEXT("Render sequences in order this to be available")))
			.ColorAndOpacity(FLinearColor::Red)
			.Text(FText::FromString(TEXT("Not found")));
		}
		
		return SNew(SBox)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		[
			SNew(SHyperlink)
			.Text(FText::FromString(FilePath))
			.OnNavigate_Lambda([=]()
			{
				// ignore if we currently rendering sequences
				if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
				{
					return;
				}
				
				FPlatformProcess::LaunchFileInDefaultExternalApplication(*FilePath);
			})
		];
	}

	if (InColumnName == TEXT("Note"))
	{
		const FLinearColor Color = FGamedevHelperEditorStyle::GetColorByStatus(ListItem->Status);

		return SNew(STextBlock).ColorAndOpacity(Color).Text(FText::FromString(ListItem->Note));
	}

	return SNew(STextBlock).Text(FText::FromString(""));
}
