// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBet.h"
#include "BatchEncodeTool/GdhBetSettings.h"
#include "BatchEncodeTool/CustomAssets/GdhBetRenderList.h"
#include "GdhCmds.h"
#include "GdhConstants.h"
#include "GdhLibEditor.h"
#include "IContentBrowserSingleton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhBet::Construct(const FArguments& InArgs) {

	UGdhBetSettings* Settings = GetMutableDefault<UGdhBetSettings>();
	if (!Settings) return;

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCmds::Get().BetEncodePresetRefresh,
		FExecuteAction::CreateRaw(this, &SGdhBet::OnEncodePresetRefresh)
	);
	Cmds->MapAction(
		FGdhCmds::Get().BetEncodePresetCreate,
		FExecuteAction::CreateRaw(this, &SGdhBet::OnEncodePresetCreate)
	);

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

	PathCurrent = GdhConstants::PathRoot.ToString();
	Filter.ClassNames.Add(UGdhBetRenderList::StaticClass()->GetFName());

	FilterUpdate();

	FAssetPickerConfig AssetPickerConfig;
	AssetPickerConfig.Filter = Filter;
	AssetPickerConfig.bAddFilterUI = false;
	AssetPickerConfig.SetFilterDelegates.Add(&DelegateFilter);

	const auto ContentBrowserView =
		UGdhLibEditor::GetModuleContentBrowser().Get().CreateAssetPicker(AssetPickerConfig);

	FPathPickerConfig PathPickerConfig;
	PathPickerConfig.bAllowContextMenu = false;
	PathPickerConfig.bAddDefaultPath = true;
	PathPickerConfig.DefaultPath = PathCurrent;
	PathPickerConfig.OnPathSelected.BindRaw(this, &SGdhBet::OnPathSelected);

	const auto PathPickerView =
		UGdhLibEditor::GetModuleContentBrowser().Get().CreatePathPicker(PathPickerConfig);

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
			+ SSplitter::Slot().Value(0.3f)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SettingsProperty
					]
					+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
					[
						SNew(SSplitter)
						.PhysicalSplitterHandleSize(3.0f)
						.Style(FEditorStyle::Get(), "DetailsView.Splitter")
						.Orientation(Orient_Horizontal)
						+ SSplitter::Slot().Value(0.4f)
						[

							PathPickerView
						]
						+ SSplitter::Slot().Value(0.6f)
						[
							ContentBrowserView
						]
					]
				]
			]
			+ SSplitter::Slot().Value(0.3f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					CreateToolbarPresets()
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SSeparator).Thickness(3.0f)
				]
			]
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(STextBlock).Text(FText::FromString("Queue items here"))
				]
			]
		]
	];
	// clang-format on
}

void SGdhBet::OnEncodePresetRefresh() {}

void SGdhBet::OnEncodePresetCreate() {}

void SGdhBet::OnPathSelected(const FString& InPath) {
	PathCurrent = InPath;
	FilterUpdate();
}

void SGdhBet::FilterUpdate() {

	Filter.PackagePaths.Empty();
	Filter.PackagePaths.Add(FName {*PathCurrent});

	if (DelegateFilter.IsBound()) {
		DelegateFilter.Execute(Filter);
	}
}

TSharedRef<SWidget> SGdhBet::CreateToolbarPresets() const {
	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhSectionPresets");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetEncodePresetRefresh);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().BetEncodePresetCreate);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}
