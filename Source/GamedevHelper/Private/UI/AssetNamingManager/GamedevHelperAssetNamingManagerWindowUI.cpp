// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerWindowUI.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingConvention.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerListRow.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "GamedevHelper.h"
#include "GamedevHelperAssetNamingManagerLibrary.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"
#include "AssetRegistryModule.h"
#include "Engine/MapBuildDataRegistry.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

void SAssetNamingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	Settings = GetMutableDefault<UGamedevHelperAssetNamingManagerSettings>();
	NamingConvention = GetMutableDefault<UGamedevHelperAssetNamingConvention>();
	
	Settings->OnSettingsChangeDelegate.BindRaw(this, &SAssetNamingManagerWindow::ListRefresh);
	NamingConvention->OnConventionPropertyChangeDelegate.BindRaw(this, &SAssetNamingManagerWindow::ListRefresh);

	FDetailsViewArgs ViewArgs;
	ViewArgs.bUpdatesFromSelection = false;
	ViewArgs.bLockable = false;
	ViewArgs.bShowScrollBar = true;
	ViewArgs.bAllowSearch = false;
	ViewArgs.bShowOptions = false;
	ViewArgs.bAllowFavoriteSystem = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.ViewIdentifier = "AssetNamingManagerSettings";

	FDetailsViewArgs ViewArgs1;
	ViewArgs1.bUpdatesFromSelection = false;
	ViewArgs1.bLockable = false;
	ViewArgs1.bShowScrollBar = true;
	ViewArgs1.bAllowSearch = false;
	ViewArgs1.bShowOptions = false;
	ViewArgs1.bAllowFavoriteSystem = false;
	ViewArgs1.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs1.ViewIdentifier = "AssetNamingConvention";


	const TSharedPtr<IDetailsView> SettingsProperty = PropertyEditor.CreateDetailView(ViewArgs);
	const TSharedPtr<IDetailsView> NamingConventionProperty = PropertyEditor.CreateDetailView(ViewArgs1);
	
	SettingsProperty->SetObject(Settings);
	NamingConventionProperty->SetObject(NamingConvention);

	SortColumn = TEXT("AssetClass");

	ListUpdate();

	ChildSlot
	[
		SNew(SSplitter)
		.Style(FEditorStyle::Get(), "ContentBrowser.Splitter")
		.Orientation(Orient_Horizontal)
		.PhysicalSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(0.4f)
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
					SettingsProperty.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					NamingConventionProperty.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#4CAF50"))})
						.ContentPadding(FMargin{0})
						.OnClicked_Raw(this, &SAssetNamingManagerWindow::OnRenameBtnClick)
						.IsEnabled_Raw(this, &SAssetNamingManagerWindow::IsRenameBtnEnabled)
						[
							SNew(STextBlock)
							.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light20"))
							.Text(FText::FromString(TEXT("Rename Assets")))
						]
					]
					+ SHorizontalBox::Slot()
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#9E9E9E"))})
						.ContentPadding(FMargin{0})
						.OnClicked_Raw(this, &SAssetNamingManagerWindow::OnRefreshBtnClick)
						[
							SNew(STextBlock)
							.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light20"))
							.Text(FText::FromString(TEXT("Refresh")))
						]
					]
				]
			]
		]
		+ SSplitter::Slot()
		.Value(0.6f)
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
				  .AutoHeight()
				[
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>)
						.ListItemsSource(&AssetList)
						.SelectionMode(ESelectionMode::Multi)
						.OnGenerateRow(this, &SAssetNamingManagerWindow::OnGenerateRow)
						// .OnContextMenuOpening(this, &SVirtualGamesRendererWindow::OnContextMenu)
						// .OnMouseButtonDoubleClick_Raw(this, &SVirtualGamesRendererWindow::OnRowMouseDblClick)
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
								.Text(LOCTEXT("Preview", "#"))
							]
							+ SHeaderRow::Column(FName("AssetClass"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Center)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FixedWidth(200.0f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("AssetClass", "Type"))
							]
							+ SHeaderRow::Column(FName("Result"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Left)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FillWidth(0.4f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Result", "Naming Preview"))
							]
							+ SHeaderRow::Column(FName("Path"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Left)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FillWidth(0.3f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Path", "Path"))
							]
							+ SHeaderRow::Column(FName("Note"))
							  // .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Center)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FillWidth(0.3f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Note", "Note"))
							]
						)
					]
				]
			]
		]
	];
}

void SAssetNamingManagerWindow::ListUpdate()
{
	bRenameBtnActive = true;
	AssetList.Reset();
	
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

	FARFilter Filter;
	Filter.PackagePaths.Add(FName{*Settings->ScanPath.Path});
	Filter.bRecursivePaths = Settings->bScanRecursive;
	Filter.bRecursiveClasses = true;
	Filter.RecursiveClassesExclusionSet.Add(UWorld::StaticClass()->GetFName());
	Filter.RecursiveClassesExclusionSet.Add(UMapBuildDataRegistry::StaticClass()->GetFName());

	if (!Settings->bShowMissingTypes)
	{
		TArray<UClass*> ConventionAssetClasses;
		NamingConvention->GetAssetClasses(ConventionAssetClasses);
		
		for (const auto& AssetClass : ConventionAssetClasses)
		{
			Filter.ClassNames.Add(AssetClass->GetFName());
		}
	}

	TArray<FAssetData> Assets;
	AssetRegistryModule.Get().GetAssets(Filter, Assets);

	if (Assets.Num() == 0)
	{
		bRenameBtnActive = false;
		return;
	}

	UGamedevHelperAssetNamingManagerLibrary::GetRenamePreviews(Assets, NamingConvention, RenamePreviews);

	if (RenamePreviews.Num() == 0)
	{
		bRenameBtnActive = false;
		return;
	}

	AssetList.Reserve(RenamePreviews.Num());

	for (const auto& RenamePreview : RenamePreviews)
	{
		if (!Settings->bShowMissingTypes && RenamePreview.GetStatus() == EGamedevHelperRenameStatus::MissingSettings)
		{
			continue;
		}

		// if (RenamePreview.GetStatus() == EGamedevHelperRenameStatus::OK)
		// {
		// 	continue;
		// }
		//
		UGamedevHelperAssetNamingListItem* Data = NewObject<UGamedevHelperAssetNamingListItem>();

		Data->OldName = RenamePreview.GetOldName();
		Data->NewName = RenamePreview.GetNewName();
		Data->AssetData = RenamePreview.GetAssetData();
		Data->Note = RenamePreview.GetStatusMsg();
		Data->Status = RenamePreview.GetStatus();

		if (!RenamePreview.IsValid())
		{
			bRenameBtnActive = false;
		}

		AssetList.Add(Data);
	}

	if (AssetList.Num() == 0)
	{
		bRenameBtnActive = false;
	}

	ListSort();
}

void SAssetNamingManagerWindow::ListSort()
{
	// todo:ashe23 fix sorting, when just updating naming case, list must not change its order

	if (CurrentSortMode == EColumnSortMode::Ascending)
	{
		AssetList.Sort([&](const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data1,
		                   const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare(Data2->AssetData.AssetName) < 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare(Data2->AssetData.PackagePath) < 0;
			}

			return Data1->AssetData.AssetClass.Compare(Data2->AssetData.AssetClass) < 0;
		});
	}

	if (CurrentSortMode == EColumnSortMode::Descending)
	{
		AssetList.Sort([&](const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data1,
		                   const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare(Data2->AssetData.AssetName) > 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare(Data2->AssetData.PackagePath) > 0;
			}

			return Data1->AssetData.AssetClass.Compare(Data2->AssetData.AssetClass) > 0;
		});
	}
}

void SAssetNamingManagerWindow::ListRefresh()
{
	ListUpdate();

	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
}

TSharedRef<ITableRow> SAssetNamingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGamedevHelperAssetNamingListItem> InItem,
                                                               const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperAssetNamingListItem, OwnerTable).RowItem(InItem);
}

FReply SAssetNamingManagerWindow::OnRenameBtnClick()
{
	TArray<FAssetData> AssetsList;
	AssetsList.Reserve(AssetList.Num());

	for (const auto& Asset : AssetList)
	{
		AssetsList.Add(Asset->AssetData);
	}

	UGamedevHelperAssetNamingManagerLibrary::RenameAssets(AssetsList);

	ListRefresh();

	return FReply::Handled();
}

FReply SAssetNamingManagerWindow::OnRefreshBtnClick()
{
	ListRefresh();

	return FReply::Handled();
}

bool SAssetNamingManagerWindow::IsRenameBtnEnabled() const
{
	return bRenameBtnActive;
}

void SAssetNamingManagerWindow::OnSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode)
{
	CurrentSortMode = CurrentSortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;
	SortColumn = Name;

	ListRefresh();
}

#undef LOCTEXT_NAMESPACE
