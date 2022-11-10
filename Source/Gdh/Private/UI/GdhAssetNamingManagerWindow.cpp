// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhAssetNamingManagerWindow.h"
#include "UI/GdhAssetNamingManagerListItem.h"
#include "UI/GdhAssetNamingManagerRenamePreview.h"
#include "GdhStyles.h"
#include "Settings/GdhAssetNamingConventionSettings.h"
// Engine Headers
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/MapBuildDataRegistry.h"
#include "Libs/GdhAssetLibrary.h"
#include "Libs/GdhStringLibrary.h"
#include "Misc/ScopedSlowTask.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhAssetNamingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	ConventionSettings = GetMutableDefault<UGdhAssetNamingConventionSettings>();
	ScanSettings = GetMutableDefault<UGdhAssetNamingManagerScanSettings>();

	check(ConventionSettings.Get());
	check(ScanSettings.Get());

	ConventionSettings->OnChange().BindLambda([&]()
	{
		ListUpdate();
	});

	ScanSettings->OnChange().BindLambda([&]()
	{
		ListUpdate();
	});

	FDetailsViewArgs ViewArgs;
	ViewArgs.bUpdatesFromSelection = false;
	ViewArgs.bLockable = false;
	ViewArgs.bShowScrollBar = true;
	ViewArgs.bAllowSearch = true;
	ViewArgs.bShowOptions = false;
	ViewArgs.bAllowFavoriteSystem = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.ViewIdentifier = "AssetNamingManagerSettings";

	const TSharedPtr<IDetailsView> ScanSettingsProperty = PropertyEditor.CreateDetailView(ViewArgs);
	ScanSettingsProperty->SetObject(ScanSettings.Get());

	ViewArgs.ViewIdentifier = "AssetNamingConventionSettings";
	const TSharedPtr<IDetailsView> ConventionSettingsProperty = PropertyEditor.CreateDetailView(ViewArgs);
	ConventionSettingsProperty->SetObject(ConventionSettings.Get());

	ListUpdate();

	ChildSlot
	[
		SNew(SSplitter)
		.Style(&FAppStyle::Get().GetWidgetStyle<FSplitterStyle>("Splitter"))
		.Orientation(Orient_Horizontal)
		.PhysicalSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(0.3f)
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
				  .AutoHeight()
				[
					ScanSettingsProperty.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					ConventionSettingsProperty.ToSharedRef()
				]
			]
		]
		+ SSplitter::Slot()
		[
			SNew(SScrollBox)
			.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
			.AnimateWheelScrolling(true)
			.AllowOverscroll(EAllowOverscroll::No)
			+ SScrollBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .AutoHeight()
				  .Padding(FMargin{10.0f})
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					  .FillWidth(1.0)
					  .HAlign(HAlign_Left)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
					.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
					.ContentPadding(FMargin{5})
					// .OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnOutputDirOpenClick)
					// .IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnOutputDirOpenEnabled)
					.ToolTipText(FText::FromString(TEXT("Refresh assets list")))
						[
							SNew(SImage)
							.Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Refresh"))
						]
					]
					+ SHorizontalBox::Slot()
					  .AutoWidth()
					  .HAlign(HAlign_Right)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
					// .ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
					.ContentPadding(FMargin{5})
					.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#45d94d"))})
					// .OnClicked_Raw(this, &SGdhRenderingManagerWindow::OnBtnOutputDirOpenClick)
					// .IsEnabled_Raw(this, &SGdhRenderingManagerWindow::IsBtnOutputDirOpenEnabled)
					.ToolTipText(FText::FromString(TEXT("Rename assets")))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Rename All")))
							// SNew(SImage)
							// .Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Render")) // todo:ashe23 find appropriate button icon for rename assets
						]
					]
				]
				+ SVerticalBox::Slot()
				  .AutoHeight()
				  .Padding(FMargin{10.0f})
				[
					SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhAssetNamingManagerListItem>>)
						.ListItemsSource(&ListItems)
						.SelectionMode(ESelectionMode::Multi)
						.OnGenerateRow(this, &SGdhAssetNamingManagerWindow::OnGenerateRow)
						// .OnContextMenuOpening(this, &SAssetNamingManagerWindow::GetListContextMenu)
						// .OnMouseButtonDoubleClick_Static(&SAssetNamingManagerWindow::OnListItemDblClick)
						.HeaderRow
					                                                                               (
						                                                                               SNew(SHeaderRow)
						                                                                               + SHeaderRow::Column(FName("Preview"))
						                                                                                 .HAlignCell(HAlign_Center)
						                                                                                 .VAlignCell(VAlign_Center)
						                                                                                 .HAlignHeader(HAlign_Center)
						                                                                                 .HeaderContentPadding(FMargin(10.0f))
						                                                                                 .FixedWidth(50.0f)
						                                                                               [
							                                                                               SNew(STextBlock)
							                                                                               .Text(FText::FromString(TEXT("#")))
						                                                                               ]
						                                                                               + SHeaderRow::Column(FName("AssetClass"))
						                                                                                 // .OnSort_Raw(this, &SAssetNamingManagerWindow::OnListSort)
						                                                                                 .HAlignCell(HAlign_Center)
						                                                                                 .VAlignCell(VAlign_Center)
						                                                                                 .HAlignHeader(HAlign_Center)
						                                                                                 .HeaderContentPadding(FMargin(10.0f))
						                                                                                 .FixedWidth(200.0f)
						                                                                               [
							                                                                               SNew(STextBlock)
							                                                                               .Text(FText::FromString(TEXT("Type")))
						                                                                               ]
						                                                                               + SHeaderRow::Column(FName("Result"))
						                                                                                 // .OnSort_Raw(this, &SAssetNamingManagerWindow::OnListSort)
						                                                                                 .HAlignCell(HAlign_Left)
						                                                                                 .VAlignCell(VAlign_Center)
						                                                                                 .HAlignHeader(HAlign_Center)
						                                                                                 .HeaderContentPadding(FMargin(10.0f))
						                                                                                 .FillWidth(0.4f)
						                                                                               [
							                                                                               SNew(STextBlock)
							                                                                               .Text(FText::FromString(TEXT("Rename Preview")))
						                                                                               ]
						                                                                               + SHeaderRow::Column(FName("Path"))
						                                                                                 // .OnSort_Raw(this, &SAssetNamingManagerWindow::OnListSort)
						                                                                                 .HAlignCell(HAlign_Left)
						                                                                                 .VAlignCell(VAlign_Center)
						                                                                                 .HAlignHeader(HAlign_Center)
						                                                                                 .HeaderContentPadding(FMargin(10.0f))
						                                                                                 .FillWidth(0.3f)
						                                                                               [
							                                                                               SNew(STextBlock)
							                                                                               .Text(FText::FromString(TEXT("Path")))
						                                                                               ]
						                                                                               + SHeaderRow::Column(FName("Note"))
						                                                                                 // .OnSort_Raw(this, &SAssetNamingManagerWindow::OnListSort)
						                                                                                 .HAlignCell(HAlign_Center)
						                                                                                 .VAlignCell(VAlign_Center)
						                                                                                 .HAlignHeader(HAlign_Center)
						                                                                                 .HeaderContentPadding(FMargin(10.0f))
						                                                                                 .FillWidth(0.3f)
						                                                                               [
							                                                                               SNew(STextBlock)
							                                                                               .Text(FText::FromString(TEXT("Note")))
						                                                                               ]
					                                                                               )
				]
			]
		]
	];
}

void SGdhAssetNamingManagerWindow::ListUpdate()
{
	TArray<FAssetData> Assets;

	FARFilter Filter;
	Filter.PackagePaths.Add(FName{*ScanSettings->ScanPath.Path});
	Filter.bRecursivePaths = ScanSettings->bScanRecursive;
	Filter.bRecursiveClasses = true;
	Filter.RecursiveClassesExclusionSet.Add(UMapBuildDataRegistry::StaticClass()->GetFName());
	
	UGdhAssetLibrary::GetAssetsByFilter(Filter, Assets);

	if (Assets.Num() == 0) return;

	ListItems.Reset();
	ListItems.Reserve(Assets.Num());

	FScopedSlowTask SlowTask(
		Assets.Num(),
		FText::FromString("Scanning...")
	);
	SlowTask.MakeDialog(false, false);

	for (const auto& Asset : Assets)
	{
		SlowTask.EnterProgressFrame(1.0f);

		if (!Asset.GetAsset()) continue;

		TWeakObjectPtr<UGdhAssetNamingManagerListItem> NewItem = NewObject<UGdhAssetNamingManagerListItem>();
		if (!NewItem.Get()) continue;

		NewItem->AssetData = Asset;
		NewItem->OldName = Asset.GetAsset()->GetName();
		NewItem->NewName = UGdhStringLibrary::Normalize(NewItem->OldName);
		NewItem->Note = TEXT("OK"); // todo:ashe23
		NewItem->RenameStatus = EGdhRenameStatus::Ok; // todo:ashe23

		ListItems.Add(NewItem);
	}

	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
}

void SGdhAssetNamingManagerWindow::ListSort()
{
	if (CurrentSortMode == EColumnSortMode::Ascending)
	{
		ListItems.Sort([&](const TWeakObjectPtr<UGdhAssetNamingManagerListItem>& Data1,
		                   const TWeakObjectPtr<UGdhAssetNamingManagerListItem>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare(Data2->AssetData.AssetName) < 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare(Data2->AssetData.PackagePath) < 0;
			}

			if (SortColumn.IsEqual(TEXT("Note")))
			{
				return Data1->Note.Compare(Data2->Note) < 0;
			}

			return Data1->AssetData.AssetClass.Compare(Data2->AssetData.AssetClass) < 0;
		});
	}

	if (CurrentSortMode == EColumnSortMode::Descending)
	{
		ListItems.Sort([&](const TWeakObjectPtr<UGdhAssetNamingManagerListItem>& Data1,
		                   const TWeakObjectPtr<UGdhAssetNamingManagerListItem>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare(Data2->AssetData.AssetName) > 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare(Data2->AssetData.PackagePath) > 0;
			}

			if (SortColumn.IsEqual(TEXT("Note")))
			{
				return Data1->Note.Compare(Data2->Note) > 0;
			}

			return Data1->AssetData.AssetClass.Compare(Data2->AssetData.AssetClass) > 0;
		});
	}
}

void SGdhAssetNamingManagerWindow::OnListSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode)
{
	CurrentSortMode = CurrentSortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;
	SortColumn = Name;

	ListUpdate();
}

void SGdhAssetNamingManagerWindow::UpdateRenamePreviews(const TArray<FAssetData>& Assets)
{
	if (Assets.Num() == 0) return;

	RenamePreviews.Reset();
	RenamePreviews.Reserve(Assets.Num());

	for (const auto& Asset : Assets)
	{
		if (!Asset.IsValid()) continue;

		FGdhRenamePreview RenamePreview;
		RenamePreview.SetAssetData(Asset);
	}
}

FGdhAssetAppendix SGdhAssetNamingManagerWindow::GetNameFormatByAssetData(const FAssetData& Asset) const
{
	// if (!Asset.IsValid())
	// {
	// 	return FGdhAssetFormat{};
	// }
	//
	// if (!Asset.AssetClass.IsEqual(TEXT("Blueprint")))
	// {
	// 	return GetNameFormatByClass(Asset.GetClass());
	// }
	//
	// const UClass* ParentClass = UGdhAssetLibrary::GetBlueprintParentClass(Asset);
	// const UClass* SearchClass = ParentClass ? ParentClass : UBlueprint::StaticClass();
	// FGdhAssetFormat NameFormat = GetNameFormatByClass(SearchClass);
	//
	// if (NameFormat.Prefix.IsEmpty())
	// {
	// 	const auto BlueprintType = UGdhAssetLibrary::GetBlueprintType(Asset);
	// 	const auto BlueprintTypePrefix = BlueprintTypePrefixes.Find(BlueprintType);
	// 	const FString BlueprintPrefix = BlueprintTypePrefix ? *BlueprintTypePrefix : TEXT("BP");
	// 	NameFormat.Prefix = BlueprintPrefix;
	// }
	//
	// return NameFormat;
	return {};
}

FGdhAssetAppendix SGdhAssetNamingManagerWindow::GetNameFormatByClass(const UClass* SearchClass) const
{
	// if (!SearchClass)
	// {
	// 	return FGamedevHelperAssetNameFormat{};
	// }
	//
	// for (const auto& Naming : Mappings)
	// {
	// 	const UClass* NamingClass = Naming.Key;
	// 	if (NamingClass == SearchClass)
	// 	{
	// 		return Naming.Value;
	// 	}
	// }
	//
	// return FGamedevHelperAssetNameFormat{};
	return {};
}

TSharedRef<ITableRow> SGdhAssetNamingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGdhAssetNamingManagerListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return
		SNew(SGdhAssetNamingManagerListItem, OwnerTable)
		.ListItem(InItem)
		.ToolTipText(FText::FromString(TEXT("Double click on row to see asset location in content browser")));
}
