// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "Settings/GdhMovieRenderSettings.h"
#include "Settings/GdhRenderingSettings.h"
// Engine Headers
#include "GdhStyles.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	MovieRenderSettings = GetMutableDefault<UGdhMovieRenderSettings>();

	check(RenderingSettings);
	check(MovieRenderSettings);

	RenderingSettings->OnSettingChanged().AddLambda([&](const UObject* Obj, const FPropertyChangedEvent& ChangedEvent)
	{
		UE_LOG(LogGdh, Warning, TEXT("Rendering Settings changed!"));
	});

	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhRenderingSettings";

	const TSharedPtr<IDetailsView> RenderingSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingSettingsProperty->SetObject(RenderingSettings);

	DetailsViewArgs.ViewIdentifier = "GdhMovieRenderSettings";
	const TSharedPtr<IDetailsView> MovieRenderSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	MovieRenderSettingsProperty->SetObject(MovieRenderSettings);

	ChildSlot
	[
		SNew(SWidgetSwitcher)
		.WidgetIndex_Raw(this, &SGdhRenderingManagerWindow::GetActiveWidgetIndex)
		+ SWidgetSwitcher::Slot()
		  .HAlign(HAlign_Fill)
		  .VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Center)
			[
				SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text(FText::FromString(TEXT("MovieRender currently working...")))
					.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light30"))
			]
		]
		+ SWidgetSwitcher::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			  .Padding(FMargin{10.0f})
			  .FillHeight(0.95f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					RenderingSettingsProperty.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					MovieRenderSettingsProperty.ToSharedRef()
				]
			]
			+ SVerticalBox::Slot()
			  .Padding(FMargin{10.0f})
			  .FillHeight(0.05f)
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
					.Visibility_Raw(this, &SGdhRenderingManagerWindow::GetConsoleBoxVisibility)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Left)
						.Text_Raw(this, &SGdhRenderingManagerWindow::GetConsoleBoxText)
						.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Light15"))
						.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
					]
				]
			]
		]
	];
}

int32 SGdhRenderingManagerWindow::GetActiveWidgetIndex() const
{
	if (!GEditor) return WidgetIndexRenderingManagerWorking;

	if (GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering()) return WidgetIndexRenderingManagerWorking;

	return WidgetIndexRenderingManagerUI;
}

FText SGdhRenderingManagerWindow::GetConsoleBoxText() const
{
	if (RenderingSettings->OutputDirectory.Path.IsEmpty())
	{
		return FText::FromString(TEXT("Output directory not specified"));
	}

	if (!FPaths::DirectoryExists(RenderingSettings->OutputDirectory.Path))
	{
		return FText::FromString(TEXT("Output directory does not exist"));
	}

	if (RenderingSettings->FFmpegExe.FilePath.IsEmpty())
	{
		return FText::FromString(TEXT("FFmpeg.exe path not specified. Must be absolute path to exe, or just can be ffmpeg.exe if you have it in system PATHS"));
	}

	if (!FPaths::FileExists(RenderingSettings->FFmpegExe.FilePath))
	{
		return FText::FromString(FString::Printf(TEXT("Cant find ffmpeg.exe at given '%s' location"), *RenderingSettings->FFmpegExe.FilePath));
	}

	if (RenderingSettings->GetResolution().X % 2 != 0 || RenderingSettings->GetResolution().Y % 2 != 0)
	{
		return FText::FromString(TEXT("Resolution must have dimensions divisible by 2"));
	}

	return FText::FromString(TEXT(""));
}

EVisibility SGdhRenderingManagerWindow::GetConsoleBoxVisibility() const
{
	return RenderingSettings->IsValidSettings() ? EVisibility::Hidden : EVisibility::Visible;
}
