// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/Renderer/GamedevHelperRendererUI.h"
#include "UI/Renderer/GamedevHelperRendererSettings.h"
#include "UI/GamedevHelperEditorStyle.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

void SGamedevHelperRendererUI::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RendererSettings = GetMutableDefault<UGamedevHelperRendererSettings>();
	verify(RendererSettings);

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowScrollBar = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GamedevHelperRendererSettings";

	const TSharedPtr<IDetailsView> RendererSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RendererSettingsProperty->SetObject(RendererSettings);
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(FMargin{10.0f})
		.FillHeight(0.95f)
		[
			SNew(SSplitter)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			.PhysicalSplitterHandleSize(5.0f)
			+ SSplitter::Slot()
			.Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(FMargin{10.0f})
				.AutoHeight()
				[
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.Padding(FMargin{10.0f})
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							RendererSettingsProperty.ToSharedRef()
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		.Padding(FMargin{10.0f,5.0f})
		.FillHeight(0.05f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.Visibility_Raw(this, &SGamedevHelperRendererUI::GetConsoleBoxVisibility)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Left)
				.Text_Raw(this, &SGamedevHelperRendererUI::GetConsoleBoxText)
				.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("VirtualGames.Font.Light15"))
				.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#E53935"))})
			]
		]
	];
}

FText SGamedevHelperRendererUI::GetConsoleBoxText() const
{
	// IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (RendererSettings->FFmpegExePath.FilePath.IsEmpty())
	{
		return FText::FromString(TEXT("Error: FFmpeg exe path not specified"));
	}

	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(RendererSettings->FFmpegExePath.FilePath)))
	{
		return FText::FromString(TEXT("Error: FFmpeg exe path does not exist"));
	}

	if (RendererSettings->OutputDir.Path.IsEmpty())
	{
		return FText::FromString(TEXT("Error: Output directory not specified"));
	}

	if (!FPaths::DirectoryExists(FPaths::ConvertRelativePathToFull(RendererSettings->OutputDir.Path)))
	{
		return FText::FromString(TEXT("Error: Output directory does not exist"));
	}

	return FText::FromString(TEXT(""));
}

EVisibility SGamedevHelperRendererUI::GetConsoleBoxVisibility() const
{
	return RendererSettings->IsValid() ? EVisibility::Hidden : EVisibility::Visible;
}

#undef LOCTEXT_NAMESPACE
