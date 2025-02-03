// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBetEncodePreset.h"
#include "BatchEncodeTool/CustomAssets/GdhBetEncodePreset.h"
#include "GdhLibEditor.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhBetEncodePreset::Construct(const FArguments& InArgs) {
	if (!InArgs._EncodePreset) return;

	EncodePreset = InArgs._EncodePreset;

	FPropertyEditorModule& ModuleProperty = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	const auto SettingsProperty = ModuleProperty.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(EncodePreset);

	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
		[
			// CreateToolbarMain()
			SNew(STextBlock).Text(FText::FromString("Toolbar Here"))
		]
		+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
		[
			SNew(SSeparator).Thickness(3.0f)
		]
		+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Vertical)
			+ SSplitter::Slot().Value(0.5f)
			[
				SNew(SSplitter)
				.PhysicalSplitterHandleSize(3.0f)
				.Style(FEditorStyle::Get(), "DetailsView.Splitter")
				.Orientation(Orient_Horizontal)
				+ SSplitter::Slot().Value(0.5f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SettingsProperty
					]
				]
				+ SSplitter::Slot().Value(0.5f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SNew(STextBlock).Text(FText::FromString("Tokens"))
					]
				]
			]
			+ SSplitter::Slot().Value(0.5f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0)
				[
					SNew(STextBlock).Text(FText::FromString("Encode Command Preview"))
				]
			]
		]
	];
	// clang-format on
}
