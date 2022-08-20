// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManagerWindow.h"
#include "GdhRenderingSettings.h"

void SGdhRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	check(RenderingSettings);

	RenderingSettings->Validate();

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

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(FMargin{10.0f})
		[
			SNew(SSplitter)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			.PhysicalSplitterHandleSize(5.0f)
			+ SSplitter::Slot()
			.Value(0.3f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(FMargin{10.0f})
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					RenderingSettingsProperty.ToSharedRef()
				]
			]
		]
	];
}
