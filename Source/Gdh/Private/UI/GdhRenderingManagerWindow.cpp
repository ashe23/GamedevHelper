// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerWindow.h"
#include "Settings/GdhMovieRenderSettings.h"
#include "Settings/GdhRenderingSettings.h"
// Engine Headers
#include "GdhStyles.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Notifications/SProgressBar.h"

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
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				RenderingSettingsProperty.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			  .Padding(FMargin{10.0f})
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				MovieRenderSettingsProperty.ToSharedRef()
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
