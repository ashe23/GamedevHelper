// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/Slate/SGdhAssetNamingTool.h"
#include "AssetNamingTool/GdhAssetNamingToolSettings.h"
#include "AssetNamingTool/Slate/SGdhAssetNamingToolListItem.h"
#include "GdhCmds.h"
#include "GdhLibEditor.h"
#include "GdhStyles.h"
#include "GdhStructs.h"
#include "GdhLibAsset.h"
#include "GdhLibString.h"
// Engine Headers
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/MapBuildDataRegistry.h"
#include "Misc/ScopedSlowTask.h"
// #include "Settings/ContentBrowserSettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SGdhAssetNamingTool::Construct(const FArguments& InArgs)
{
	AssetNamingToolSettings = GetMutableDefault<UGdhAssetNamingToolSettings>();
	if (!AssetNamingToolSettings.IsValid()) return;

	CurrentPath = GdhConstants::PathRoot.ToString();

	CmdsRegister();

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();
	const FContentBrowserModule& ContentBrowser = UGdhLibEditor::GetModuleContentBrowser();

	FPathPickerConfig PathPickerConfig;
	PathPickerConfig.DefaultPath = GdhConstants::PathRoot.ToString();
	PathPickerConfig.bAddDefaultPath = true;
	PathPickerConfig.bAllowClassesFolder = false;
	PathPickerConfig.bAllowContextMenu = false;
	PathPickerConfig.bAllowReadOnlyFolders = false;
	PathPickerConfig.bFocusSearchBoxWhenOpened = false;
	PathPickerConfig.OnPathSelected.BindLambda([&](const FString& InPath)
	{
		CurrentPath = InPath;
		UpdateListData();
		UpdateListView();
	});

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhAssetNamingToolSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(AssetNamingToolSettings.Get());

	UpdateListData();
	UpdateListView();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot().Value(0.2f)
			[
				SNew(SVerticalBox).IsEnabled_Lambda([&]() { return !bEditModeEnabled; })
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SettingsProperty
				]
				+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0f)
				[
					SNew(SBorder).BorderImage(FEditorStyle::GetBrush("ContentBrowser.TopBar.GroupBorder"))
					[
						ContentBrowser.Get().CreatePathPicker(PathPickerConfig)
					]
				]
			]
			+ SSplitter::Slot().Value(0.8f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					CreateToolbarMain()
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SSeparator).Thickness(3.0f)
				]
				+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0f)
				[
					SNew(SWidgetSwitcher)
					.WidgetIndex_Raw(this, &SGdhAssetNamingTool::GetWidgetIndex)
					+ SWidgetSwitcher::Slot()
					[
						SNew(SScrollBox)
						.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
						.AnimateWheelScrolling(true)
						.AllowOverscroll(EAllowOverscroll::No)
						+ SScrollBox::Slot()
						[
							SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhAssetNamingToolListItem>>)
							.ListItemsSource(&ListItems)
							.ClearSelectionOnClick(true)
							.SelectionMode(ESelectionMode::Multi)
							.OnGenerateRow(this, &SGdhAssetNamingTool::OnGenerateRow)
							.OnMouseButtonDoubleClick(this, &SGdhAssetNamingTool::OnListRowMouseDoubleClick)
							.HeaderRow(GetHeaderRow())
						]
					]
					+ SWidgetSwitcher::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right).VAlign(VAlign_Center)
						[
							SNew(SBox).WidthOverride(32.0f).HeightOverride(32.0f)
							[
								SAssignNew(InfoTextImage, SImage).Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Check").GetIcon())
							]
						]
						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(FMargin{5.0f, 2.0f, 0.0f, 0.0f}).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SAssignNew(InfoTextBlock, STextBlock)
							.Justification(ETextJustify::Center)
							.ShadowOffset(FVector2D{1.5f, 1.5f})
							.ShadowColorAndOpacity(FLinearColor::Black)
							.Font(FGdhStyles::GetFont("Bold", 18))
							.Text(FText::FromString(TEXT("No assets to rename")))
							.ColorAndOpacity(FGdhStyles::Get().GetColor("GamedevHelper.Color.Grey"))
						]
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Left).VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text_Raw(this, &SGdhAssetNamingTool::GetListSummaryTxt)
					]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text_Raw(this, &SGdhAssetNamingTool::GetListSelectionTxt)
					]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right).VAlign(VAlign_Center)
					[
						SNew(SComboButton)
						.ContentPadding(0)
						.ForegroundColor_Raw(this, &SGdhAssetNamingTool::GetListOptionsBtnForegroundColor)
						.ButtonStyle(FEditorStyle::Get(), "ToggleButton")
						.OnGetMenuContent(this, &SGdhAssetNamingTool::GetListOptionsBtnContent)
						.ButtonContent()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
							[
								SNew(SImage).Image(FEditorStyle::GetBrush("GenericViewButton"))
							]
							+ SHorizontalBox::Slot().AutoWidth().Padding(2.0f, 0.0f, 0.0f, 0.0f).VAlign(VAlign_Center)
							[
								SNew(STextBlock).Text(FText::FromString(TEXT("View Options")))
							]
						]
					]
				]
			]
		]
	];
}

void SGdhAssetNamingTool::UpdateListData()
{
	if (!AssetNamingToolSettings.IsValid()) return;
	if (UGdhLibEditor::GetModuleAssetRegistry().GetRegistry().IsLoadingAssets()) return;
	if (UGdhLibEditor::EditorInPlayMode()) return;
	if (!CurrentPath.StartsWith(GdhConstants::PathRoot.ToString())) return;

	// const auto ContentBrowserSettings = GetMutableDefault<UContentBrowserSettings>();
	// if (!ContentBrowserSettings) return;
	//
	// ContentBrowserSettings->SetDisplayEngineFolder(false);
	// ContentBrowserSettings->SetDisplayCppFolders(false);
	// ContentBrowserSettings->SetDisplayPluginFolders(false);
	// ContentBrowserSettings->SetDisplayL10NFolder(false);
	// ContentBrowserSettings->PostEditChange();

	ListItems.Reset();

	UGdhLibEditor::CloseAllEditors();

	if (!UGdhLibAsset::SaveAllAssets(true))
	{
		UGdhLibEditor::ShowNotificationWithOutputLog(TEXT("Failed to scan assets, because not all assets have been saved."), SNotificationItem::CS_Fail, 5.0f);
		return;
	}

	{
		TArray<FAssetData> Redirectors;
		UGdhLibAsset::GetProjectRedirectors(Redirectors);
		UGdhLibAsset::FixProjectRedirectors(Redirectors);
	}

	if (UGdhLibAsset::ProjectHasRedirectors())
	{
		UGdhLibEditor::ShowNotificationWithOutputLog(
			TEXT("Project contains redirectors that are failed to fixed automatically. Please fix them manually to proceed. Check OutputLog for more information."),
			SNotificationItem::CS_Fail,
			5.0f
		);

		TArray<FAssetData> Redirectors;
		UGdhLibAsset::GetProjectRedirectors(Redirectors);

		UE_LOG(LogTemp, Error, TEXT("Redirectors Failed To Fix: "))
		for (const auto& Redirector : Redirectors)
		{
			UE_LOG(LogTemp, Error, TEXT("%s - %s"), *Redirector.AssetName.ToString(), *Redirector.PackagePath.ToString());
		}

		return;
	}

	FScopedSlowTask SlowTask(
		2.0f,
		FText::FromString("Scanning assets...")
	);
	SlowTask.MakeDialog(false, false);
	SlowTask.EnterProgressFrame(1.0f);

	TArray<FAssetData> AssetsAll;
	TArray<FAssetData> AssetsIndirect;

	UGdhLibAsset::GetAssetByPath(CurrentPath, true, AssetsAll);
	UGdhLibAsset::GetAssetsIndirect(AssetsIndirect, true);

	TSet<UClass*> AssetClassesIgnore;
	AssetClassesIgnore.Add(UWorld::StaticClass());
	AssetClassesIgnore.Add(UMapBuildDataRegistry::StaticClass());

	ListItems.Reserve(AssetsAll.Num());

	SlowTask.EnterProgressFrame(1.0f);

	FScopedSlowTask SlowTaskAssets(
		AssetsAll.Num(),
		FText::FromString("")
	);
	SlowTaskAssets.MakeDialog(false, false);

	for (const auto& Asset : AssetsAll)
	{
		SlowTaskAssets.EnterProgressFrame(1.0f, FText::FromName(Asset.AssetName));

		if (AssetsIndirect.Contains(Asset)) continue;
		if (AssetClassesIgnore.Contains(Asset.GetClass())) continue;

		TWeakObjectPtr<UGdhAssetNamingToolListItem> NewItem = NewObject<UGdhAssetNamingToolListItem>();
		if (!NewItem.IsValid()) continue;

		NewItem->AssetData = Asset;

		const FGdhAssetNameAffix Affix = UGdhLibAsset::GetAssetNameAffix(Asset, AssetNamingToolSettings->Mappings.LoadSynchronous(), AssetNamingToolSettings->BlueprintTypes);
		if (Affix.Prefix.IsEmpty() && Affix.Suffix.IsEmpty()) continue;

		NewItem->Prefix = Affix.Prefix;
		NewItem->Suffix = Affix.Suffix;
		NewItem->OldName = Asset.AssetName.ToString();
		ValidateItem(NewItem, NewItem->OldName);

		// we are not showing assets that already have correct name
		if (NewItem->OldName.Equals(NewItem->NewName, ESearchCase::CaseSensitive)) continue;

		ListItems.Emplace(NewItem);
	}
}

void SGdhAssetNamingTool::UpdateListView() const
{
	if (!ListView.IsValid()) return;

	ListView->ClearHighlightedItems();
	ListView->ClearSelection();
	ListView->RequestListRefresh();
}

void SGdhAssetNamingTool::UpdateListSort(const FName& ColumnName)
{
	if (!ListView.IsValid()) return;

	auto SortListItems = [&](auto& SortMode, auto SortFunc)
	{
		SortMode = SortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;

		ListItems.Sort(SortFunc);
	};

	if (ColumnName.IsEqual(TEXT("Status")))
	{
		SortListItems(ColumnSortModeStatus, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			const bool bHasStatus = Item1->bHasErrors || Item1->bDirty;

			return ColumnSortModeStatus == EColumnSortMode::Ascending ? bHasStatus : !bHasStatus;
		});
	}

	if (ColumnName.IsEqual(TEXT("Result")))
	{
		SortListItems(ColumnSortModePreview, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModePreview == EColumnSortMode::Ascending ? Item1->OldName < Item2->OldName : Item1->OldName > Item2->OldName;
		});
	}

	if (ColumnName.IsEqual(TEXT("AssetClass")))
	{
		SortListItems(ColumnSortModeClass, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModeClass == EColumnSortMode::Ascending ? Item1->AssetData.GetClass()->GetName() < Item2->AssetData.GetClass()->GetName() : Item1->AssetData.GetClass()->GetName() > Item2->AssetData.GetClass()->GetName();
		});
	}

	if (ColumnName.IsEqual(TEXT("Prefix")))
	{
		SortListItems(ColumnSortModePrefix, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModePrefix == EColumnSortMode::Ascending ? Item1->Prefix < Item2->Prefix : Item1->Prefix > Item2->Prefix;
		});
	}

	if (ColumnName.IsEqual(TEXT("Suffix")))
	{
		SortListItems(ColumnSortModeSuffix, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModeSuffix == EColumnSortMode::Ascending ? Item1->Suffix < Item2->Suffix : Item1->Suffix > Item2->Suffix;
		});
	}

	if (ColumnName.IsEqual(TEXT("Path")))
	{
		SortListItems(ColumnSortModePath, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModePath == EColumnSortMode::Ascending ? Item1->AssetData.PackagePath.ToString() < Item2->AssetData.PackagePath.ToString() : Item1->AssetData.PackagePath.ToString() > Item2->AssetData.PackagePath.ToString();
		});
	}

	ListView->RebuildList();
}

void SGdhAssetNamingTool::OnListSort(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type InSortMode)
{
	UpdateListSort(ColumnName);
}

void SGdhAssetNamingTool::OnListRowMouseDoubleClick(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item)
{
	if (!Item.IsValid()) return;

	UGdhLibEditor::OpenAssetEditor(Item->AssetData);
}

FText SGdhAssetNamingTool::GetListSummaryTxt() const
{
	return FText::FromString(FString::Printf(TEXT("Total %d assets"), ListItems.Num()));
}

FText SGdhAssetNamingTool::GetListSelectionTxt() const
{
	if (!ListView.IsValid()) return {};

	return FText::FromString(FString::Printf(TEXT("Selected %d"), ListView->GetSelectedItems().Num()));
}

FSlateColor SGdhAssetNamingTool::GetListOptionsBtnForegroundColor() const
{
	static const FName InvertedForegroundName("InvertedForeground");
	static const FName DefaultForegroundName("DefaultForeground");

	if (!ListOptionsBtn.IsValid()) return FEditorStyle::GetSlateColor(DefaultForegroundName);

	return ListOptionsBtn->IsHovered() ? FEditorStyle::GetSlateColor(InvertedForegroundName) : FEditorStyle::GetSlateColor(DefaultForegroundName);
}

TSharedRef<SWidget> SGdhAssetNamingTool::GetListOptionsBtnContent() const
{
	const TSharedPtr<FExtender> Extender;
	FMenuBuilder MenuBuilder(true, Cmds, Extender, true);


	return MenuBuilder.MakeWidget();
}

TSharedRef<ITableRow> SGdhAssetNamingTool::OnGenerateRow(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SGdhAssetNamingToolListItem, OwnerTable).ListItem(Item).OnAssetNameChange_Raw(this, &SGdhAssetNamingTool::OnItemAssetNameChanged);
}

TSharedRef<SWidget> SGdhAssetNamingTool::CreateToolbarMain() const
{
	FToolBarBuilder ToolBarBuilder{Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhAssetNamingToolMainToolbar");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().ScanAssets);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().RenameAssets);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().ClearSelection);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().EditMode);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().ApplyChanges);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().UndoChanges);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SHeaderRow> SGdhAssetNamingTool::GetHeaderRow()
{
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(TEXT("Status"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FixedWidth(20.0f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("#")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
			.ToolTipText(FText::FromName(TEXT("Status")))
		]
		+ SHeaderRow::Column(TEXT("Result"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.6f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Preview")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Path"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.3f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Path")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("AssetClass"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.1f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Class")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Prefix"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.03f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Prefix")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
			.ToolTipText(FText::FromName(TEXT("Status")))
		]
		+ SHeaderRow::Column(TEXT("Suffix"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.03f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Suffix")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
			.ToolTipText(FText::FromName(TEXT("Status")))
		];
}

int32 SGdhAssetNamingTool::GetWidgetIndex() const
{
	if (!FPaths::IsUnderDirectory(CurrentPath, GdhConstants::PathRoot.ToString()))
	{
		if (InfoTextBlock.IsValid())
		{
			InfoTextBlock->SetText(FText::FromString(TEXT("Only Assets under Content folder can be scanned")));
		}

		if (InfoTextImage.IsValid())
		{
			InfoTextImage->SetImage(FGdhStyles::GetIcon("GamedevHelper.Icon.Warning").GetIcon());
		}

		return 1;
	}

	if (InfoTextBlock.IsValid())
	{
		InfoTextBlock->SetText(FText::FromString(TEXT("No assets to rename")));
	}

	if (InfoTextImage.IsValid())
	{
		InfoTextImage->SetImage(FGdhStyles::GetIcon("GamedevHelper.Icon.Check").GetIcon());
	}

	return ListItems.Num() == 0 ? 1 : 0;
}

void SGdhAssetNamingTool::GetDirtyItems(TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>>& Items)
{
	Items.Reset();

	for (const auto& Item : ListItems)
	{
		if (!Item.IsValid()) continue;

		if (Item->bDirty)
		{
			Items.Add(Item);
		}
	}
}

void SGdhAssetNamingTool::ToggleEditMode(const bool bEnable)
{
	for (const auto& Item : ListItems)
	{
		if (!Item.IsValid()) continue;

		Item->bEditMode = bEnable;
	}

	if (ListView)
	{
		ListView->RebuildList();
	}
}

void SGdhAssetNamingTool::OnItemAssetNameChanged(const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item, const FString& Name) const
{
	if (!Item.IsValid()) return;

	ValidateItem(Item, Name);

	Item->bDirty = !Item->AssetData.AssetName.ToString().Equals(Item->NewName, ESearchCase::CaseSensitive);
}

void SGdhAssetNamingTool::CmdsRegister()
{
	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCmds::Get().ScanAssets,
		FExecuteAction::CreateLambda([&]()
		{
			UpdateListData();
			UpdateListView();
		}),
		FCanExecuteAction::CreateLambda([&] { return !bEditModeEnabled; })
	);
	Cmds->MapAction(
		FGdhCmds::Get().RenameAssets,
		FExecuteAction::CreateLambda([&]()
		{
			if (!ListView.IsValid()) return;

			const FText Title = FText::FromString(TEXT("Asset Naming Tool"));
			const FText Context = FText::FromString(TEXT("Are you sure you want to rename assets?"));

			const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
			if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;

			const auto SelectedItems = ListView->GetSelectedItems();
			const auto Items = SelectedItems.Num() > 0 ? SelectedItems : ListItems;

			TMap<FAssetData, FString> AssetsToRename;
			AssetsToRename.Reserve(Items.Num());

			for (const auto& Item : Items)
			{
				if (!Item.IsValid()) continue;
				if (Item->bHasErrors) continue;

				AssetsToRename.Add(Item->AssetData, Item->NewName);
			}

			RenameAssets(AssetsToRename);
			UpdateListData();
			UpdateListView();
		}),
		FCanExecuteAction::CreateLambda([&]
		{
			return !bEditModeEnabled && ListItems.Num() > 0;
		})
	);

	Cmds->MapAction(
		FGdhCmds::Get().ClearSelection,
		FExecuteAction::CreateLambda([&]() { UpdateListView(); }),
		FCanExecuteAction::CreateLambda([&]() { return ListView && ListView->GetSelectedItems().Num() > 0; })
	);

	Cmds->MapAction(
		FGdhCmds::Get().EditMode,
		FExecuteAction::CreateLambda([&]()
		{
			bEditModeEnabled = true;
			ToggleEditMode(true);
		}),
		FCanExecuteAction::CreateLambda([&]()
		{
			return !bEditModeEnabled && ListItems.Num() > 0;
		}),
		FIsActionChecked::CreateLambda([&]() { return bEditModeEnabled; })
	);

	Cmds->MapAction(
		FGdhCmds::Get().ApplyChanges,
		FExecuteAction::CreateLambda([&]()
		{
			TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>> DirtyItems;
			GetDirtyItems(DirtyItems);

			if (DirtyItems.Num() > 0)
			{
				const FText Title = FText::FromString(TEXT("Asset Naming Tool"));
				const FText Context = FText::FromString(FString::Printf(TEXT("Are you sure you want to rename %d modified assets?"), DirtyItems.Num()));
				const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
				if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;
			}

			TMap<FAssetData, FString> AssetsToRename;
			AssetsToRename.Reserve(DirtyItems.Num());

			for (const auto& Item : DirtyItems)
			{
				if (!Item.IsValid()) continue;
				if (Item->bHasErrors) continue;

				AssetsToRename.Add(Item->AssetData, Item->NewName);
			}

			bEditModeEnabled = false;
			RenameAssets(AssetsToRename);
			UpdateListData();
			UpdateListView();
		}),
		FCanExecuteAction::CreateLambda([&]()
		{
			TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>> DirtyItems;
			GetDirtyItems(DirtyItems);

			return DirtyItems.Num() > 0;
		}),
		FIsActionChecked::CreateLambda([&]() { return false; }),
		FIsActionButtonVisible::CreateLambda([&]() { return bEditModeEnabled; })
	);

	Cmds->MapAction(
		FGdhCmds::Get().UndoChanges,
		FExecuteAction::CreateLambda([&]()
		{
			TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>> DirtyItems;
			GetDirtyItems(DirtyItems);

			if (DirtyItems.Num() > 0)
			{
				const FText Title = FText::FromString(TEXT("Asset Naming Tool"));
				const FText Context = FText::FromString(FString::Printf(TEXT("Are you sure you want to discard all changes?")));
				const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
				if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;
			}

			bEditModeEnabled = false;
			UpdateListData();
			UpdateListView();
		}),
		FCanExecuteAction::CreateLambda([&]() { return bEditModeEnabled; }),
		FIsActionChecked::CreateLambda([&]() { return false; }),
		FIsActionButtonVisible::CreateLambda([&]() { return bEditModeEnabled; })
	);
}

void SGdhAssetNamingTool::ValidateItem(const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item, const FString& Name) const
{
	if (!Item.IsValid()) return;

	Item->bHasErrors = false;
	Item->Note.Empty();

	const FGdhAssetNameAffix Affix = UGdhLibAsset::GetAssetNameAffix(Item->AssetData, AssetNamingToolSettings->Mappings.LoadSynchronous(), AssetNamingToolSettings->BlueprintTypes);
	const FString NamePreview = UGdhLibAsset::GetAssetNameByConvention(
		Name,
		Affix,
		AssetNamingToolSettings->AssetNamingCase,
		AssetNamingToolSettings->PrefixNamingCase,
		AssetNamingToolSettings->SuffixNamingCase
	);
	Item->NewName = NamePreview;

	const FString AssetPreviewObjectPath = Item->AssetData.PackagePath.ToString() + FString::Printf(TEXT("/%s.%s"), *Item->NewName, *Item->NewName);
	const FAssetData ExistingAssetData = UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetByObjectPath(FName{*AssetPreviewObjectPath});

	if (ExistingAssetData.IsValid() && Item->OldName.Equals(Item->NewName, ESearchCase::CaseSensitive))
	{
		Item->bHasErrors = true;
		Item->Note = TEXT("Asset with same name already exists in current path");
	}
	else if (Item->NewName.IsEmpty())
	{
		Item->bHasErrors = true;
		Item->Note = TEXT("Asset name cant be empty");
	}
	else if (UGdhLibString::HasUnicode(Item->NewName))
	{
		Item->bHasErrors = true;
		Item->Note = TEXT("Asset name contains unicode characters");
	}
	else if (!UGdhLibString::HasOnly(Item->NewName, GdhConstants::ValidAssetNameChars) || !UGdhLibString::HasOnly(Name, GdhConstants::ValidAssetNameChars))
	{
		Item->bHasErrors = true;
		Item->Note = TEXT("Asset name contains invalid characters");
	}
}

void SGdhAssetNamingTool::RenameAssets(const TMap<FAssetData, FString>& Assets)
{
	if (Assets.Num() == 0)
	{
		UGdhLibEditor::ShowNotification(TEXT("No valid assets to rename"), SNotificationItem::CS_Fail, 5.0f);
		return;
	}

	FScopedSlowTask SlowTask(
		static_cast<float>(Assets.Num()),
		FText::FromString("Renaming assets...")
	);
	SlowTask.MakeDialog(false, false);

	const int32 NumTotal = Assets.Num();
	int32 NumRenamed = 0;

	for (const auto& Asset : Assets)
	{
		SlowTask.EnterProgressFrame(1.0f, FText::FromString(Asset.Value));

		if (UGdhLibAsset::RenameAsset(Asset.Key, Asset.Value))
		{
			++NumRenamed;
		}
	}

	const FString Msg = FString::Printf(TEXT("Renamed %d of %d assets"), NumRenamed, NumTotal);

	if (NumRenamed == NumTotal)
	{
		UGdhLibEditor::ShowNotification(Msg, SNotificationItem::CS_Success, 5.0f);
	}
	else
	{
		UGdhLibEditor::ShowNotificationWithOutputLog(Msg, SNotificationItem::CS_Fail, 10.0f);
	}

	TArray<FAssetData> Redirectors;
	UGdhLibAsset::GetProjectRedirectors(Redirectors);
	UGdhLibAsset::FixProjectRedirectors(Redirectors);
}
