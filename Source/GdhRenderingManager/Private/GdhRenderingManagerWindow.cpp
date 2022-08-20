// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManagerWindow.h"
#include "GdhRenderingSettings.h"
#include "GdhRenderingQueueSettings.h"

void SGdhRenderingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	RenderingQueueSettings = GetMutableDefault<UGdhRenderingQueueSettings>();
	
	check(RenderingSettings);
	check(RenderingQueueSettings);

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
	
	DetailsViewArgs.ViewIdentifier = "GdhRenderingQueueSettings";
	const TSharedPtr<IDetailsView> RenderingQueueSettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	RenderingQueueSettingsProperty->SetObject(RenderingQueueSettings);

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
				+ SVerticalBox::Slot()
				.Padding(FMargin{10.0f})
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					RenderingQueueSettingsProperty.ToSharedRef()
				]
			]
		]
	];
}
