// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/Slate/SGdhAssetNamingTool.h"
#include "AssetNamingTool/GdhAssetNamingToolSettings.h"
#include "AssetNamingTool/Slate/SGdhAssetNamingToolListItem.h"
#include "GdhCmds.h"
#include "GdhLibEditor.h"
#include "GdhStyles.h"
#include "GdhStructs.h"
#include "GdhLibAsset.h"
// Engine Headers
#include "FileHelpers.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/MapBuildDataRegistry.h"
#include "Misc/ScopedSlowTask.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SGdhAssetNamingTool::Construct(const FArguments& InArgs)
{
	AssetNamingToolSettings = GetMutableDefault<UGdhAssetNamingToolSettings>();
	if (!AssetNamingToolSettings.IsValid()) return;

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
			FText Context;

			if (ListView->GetSelectedItems().Num() == 0)
			{
				Context = FText::FromString(TEXT("Are you sure you want to rename all assets in selected folder?"));
			}
			else
			{
				Context = FText::FromString(TEXT("Are you sure you want to rename selected assets?"));
			}

			const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
			if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;

			// todo:ashe23 rename assets

			UpdateListData();
		}),
		FCanExecuteAction::CreateLambda([&] { return !bEditModeEnabled; })
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
			TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>> DirtyItems;
			GetDirtyItems(DirtyItems);

			if (DirtyItems.Num() > 0)
			{
				const FText Title = FText::FromString(TEXT("Asset Naming Tool"));
				const FText Context = FText::FromString(FString::Printf(TEXT("Are you sure you want to disable edit mode? There are %d modified assets."), DirtyItems.Num()));
				const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
				if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;
			}

			bEditModeEnabled = !bEditModeEnabled;
			ToggleEditMode(bEditModeEnabled);
		}),
		FCanExecuteAction::CreateLambda([]() { return true; }),
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

				AssetsToRename.Add(Item->AssetData, Item->NewName);
			}

			FScopedSlowTask SlowTask(
				static_cast<float>(DirtyItems.Num()),
				FText::FromString("Renaming assets...")
			);
			SlowTask.MakeDialog(false, false);

			const int32 NumTotal = AssetsToRename.Num();
			int32 NumRenamed = 0;

			for (const auto& Asset : AssetsToRename)
			{
				SlowTask.EnterProgressFrame(1.0f);

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

			bEditModeEnabled = false;
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
			const FText Title = FText::FromString(TEXT("Asset Naming Tool"));
			const FText Context = FText::FromString(FString::Printf(TEXT("Are you sure you want to discard all changes?")));
			const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
			if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;

			bEditModeEnabled = false;
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
		FGdhCmds::Get().NumerizeAssets,
		FExecuteAction::CreateLambda([&]() {}),
		FCanExecuteAction::CreateLambda([&]()
		{
			return !bEditModeEnabled;
		}),
		FIsActionChecked::CreateLambda([&]() { return false; }),
		FIsActionButtonVisible::CreateLambda([&]() { return !bEditModeEnabled; })
	);

	Cmds->MapAction(
		FGdhCmds::Get().SearchAndReplace,
		FExecuteAction::CreateLambda([&]() {}),
		FCanExecuteAction::CreateLambda([&]()
		{
			return !bEditModeEnabled;
		}),
		FIsActionChecked::CreateLambda([&]() { return false; }),
		FIsActionButtonVisible::CreateLambda([&]() { return !bEditModeEnabled; })
	);

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();
	const FContentBrowserModule& ContentBrowser = UGdhLibEditor::GetModuleContentBrowser();

	CurrentPath = GdhConstants::PathRoot.ToString();

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
								SNew(SImage).Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Check").GetIcon())
							]
						]
						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(FMargin{5.0f, 2.0f, 0.0f, 0.0f}).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
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
	ListItems.Reset();

	if (UGdhLibEditor::GetModuleAssetRegistry().GetRegistry().IsLoadingAssets()) return;

	if (!FEditorFileUtils::SaveDirtyPackages(true, true, true, false, false, false))
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

		ListItems.Empty();

		UpdateListView();

		return;
	}

	if (!AssetNamingToolSettings.IsValid()) return;

	FScopedSlowTask SlowTask(
		2.0f,
		FText::FromString("Scanning assets...")
	);
	SlowTask.MakeDialog(false, false);
	SlowTask.EnterProgressFrame(1.0f);

	TArray<FAssetData> AssetsAll;
	// todo:ashe23 think about how exactly show this type of assets
	TArray<FAssetData> AssetsUnicode;
	TArray<FAssetData> AssetsIndirect;

	UGdhLibAsset::GetAssetByPath(CurrentPath, true, AssetsAll);
	UGdhLibAsset::GetAssetsIndirect(AssetsIndirect, true);
	UGdhLibAsset::GetAssetsUnicode(AssetsUnicode, true);

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

		UGdhAssetNamingToolListItem* NewItem = NewObject<UGdhAssetNamingToolListItem>();
		if (!NewItem) continue;

		const FGdhAssetNameAffix Affix = UGdhLibAsset::GetAssetNameAffix(Asset, AssetNamingToolSettings->Mappings.LoadSynchronous(), AssetNamingToolSettings->BlueprintTypes);

		NewItem->AssetData = Asset;
		NewItem->OldName = Asset.AssetName.ToString();
		NewItem->NewName = UGdhLibAsset::GetAssetNameByConvention(
			NewItem->OldName,
			Affix,
			AssetNamingToolSettings->AssetNamingCase,
			AssetNamingToolSettings->PrefixNamingCase,
			AssetNamingToolSettings->SuffixNamingCase,
			AssetNamingToolSettings->Delimiter
		);

		// todo:ashe23 assets with conflicting name, that already exists?
		if (NewItem->OldName.Equals(NewItem->NewName, ESearchCase::CaseSensitive)) continue;

		NewItem->Prefix = Affix.Prefix;
		NewItem->Suffix = Affix.Suffix;
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

TSharedRef<SWidget> SGdhAssetNamingTool::GetListOptionsBtnContent()
{
	const TSharedPtr<FExtender> Extender;
	FMenuBuilder MenuBuilder(true, Cmds, Extender, true);

	// MenuBuilder.AddMenuEntry(
	// 	FText::FromString(TEXT("Show Unconfigured Assets")),
	// 	FText::FromString(TEXT("Show assets that dont have naming settings specified")),
	// 	FSlateIcon(),
	// 	FUIAction
	// 	(
	// 		FExecuteAction::CreateLambda([&]
	// 		{
	// 			AssetNamingToolSubsystem->bShowAssetsUnspecified = !AssetNamingToolSubsystem->bShowAssetsUnspecified;
	//
	// 			UpdateListData();
	// 			UpdateListView();
	// 		}),
	// 		FCanExecuteAction::CreateLambda([&]()
	// 		{
	// 			return AssetNamingToolSubsystem != nullptr;
	// 		}),
	// 		FGetActionCheckState::CreateLambda([&]()
	// 		{
	// 			return AssetNamingToolSubsystem->bShowAssetsUnspecified ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	// 		})
	// 	),
	// 	NAME_None,
	// 	EUserInterfaceActionType::ToggleButton
	// );

	return MenuBuilder.MakeWidget();
}

TSharedRef<ITableRow> SGdhAssetNamingTool::OnGenerateRow(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SGdhAssetNamingToolListItem, OwnerTable).ListItem(Item);
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
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().NumerizeAssets);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().SearchAndReplace);
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
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Preview")))
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
	return ListItems.Num() == 0 ? 1 : 0;
}

void SGdhAssetNamingTool::GetDirtyItems(TArray<TWeakObjectPtr<UGdhAssetNamingToolListItem>>& Items)
{
	Items.Reset();

	for (const auto& Item : ListItems)
	{
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

		if (!bEnable)
		{
			Item->bDirty = false;
		}
	}

	if (ListView)
	{
		ListView->RebuildList();
	}
}
