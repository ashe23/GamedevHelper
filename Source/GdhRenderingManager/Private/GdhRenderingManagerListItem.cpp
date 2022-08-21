// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManagerWindowListItem.h"
#include "GdhRenderingSettings.h"
#include "LevelSequence.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Widgets/Input/SHyperlink.h"
// #include "Editor/EditorEngine.h"

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
	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	
	if (InColumnName.IsEqual(TEXT("Sequence")))
	{
		const FString SequenceName = FString::Printf(
			TEXT("%s%s"),
			ListItem->MoviePipelineQueue ? *(ListItem->MoviePipelineQueue->GetName() + TEXT(" - ")) : TEXT(""),
			ListItem->LevelSequence ? *ListItem->LevelSequence->GetName() : TEXT("")
		);
		return SNew(STextBlock).Text(FText::FromString(SequenceName));
	}

	if (InColumnName.IsEqual(TEXT("RenderedFrames")))
	{
		return SNew(STextBlock).Text(FText::FromString("0"));
	}

	if (InColumnName.IsEqual(TEXT("RenderedFramesDir")))
	{
		const FString Path = RenderingSettings->GetImageOutputDir(ListItem->LevelSequence, ListItem->MoviePipelineQueue);
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
		return SNew(STextBlock).Text(FText::FromString("0"));
	}
	
	if (InColumnName.IsEqual(TEXT("RenderedVideoFile")))
	{
		const FString Path = RenderingSettings->GetVideoOutputDir(ListItem->LevelSequence, ListItem->MoviePipelineQueue);
		const FString FilePath = FString::Printf(TEXT("%s/%s.%s"), *Path, *ListItem->LevelSequence->GetName(), *RenderingSettings->GetVideoExtension());

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
	
	if (InColumnName.IsEqual(TEXT("Note")))
	{
		// todo:ashe23
		return SNew(STextBlock).Text(FText::FromString("Note"));
	}
	
	if (InColumnName.IsEqual(TEXT("Status")))
	{
		// todo:ashe23
		return SNew(STextBlock).Text(FText::FromString("Status"));
	}
	
	return SNew(STextBlock).Text(FText::FromString(""));
}
