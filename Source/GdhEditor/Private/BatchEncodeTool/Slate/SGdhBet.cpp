// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBet.h"
#include "BatchEncodeTool/GdhBetSettings.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "GdhLibEditor.h"
#include "IContentBrowserSingleton.h"

void SGdhBet::Construct(const FArguments& InArgs) {

	UGdhBetSettings* Settings = GetMutableDefault<UGdhBetSettings>();
	if (!Settings) return;

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhBetSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(Settings);

	FARFilter Filter;
	Filter.ClassNames.Add(UGdhBetRenderList::StaticClass()->GetFName());

	FAssetPickerConfig AssetPickerConfig;
	AssetPickerConfig.Filter = Filter;
	AssetPickerConfig.bAddFilterUI = false;
	const auto ContentBrowserView =
		UGdhLibEditor::GetModuleContentBrowser().Get().CreateAssetPicker(AssetPickerConfig);

	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SettingsProperty
				]
			]
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					ContentBrowserView
				]
			]
		]
	];
	// clang-format on
}
