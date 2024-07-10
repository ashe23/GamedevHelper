// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ActorNamingTool/Slate/SGdhActorNamingTool.h"
#include "ActorNamingTool/GdhActorNamingToolSettings.h"
#include "ActorNamingTool/Slate/SGdhActorNamingToolListItem.h"
#include "GdhCmds.h"
#include "GdhLibEditor.h"
// Engine Headers
#include "EditorLevelLibrary.h"
#include "GdhLibString.h"
#include "GdhStyles.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhActorNamingTool::Construct(const FArguments& InArgs)
{
	ActorNamingToolSettings = GetMutableDefault<UGdhActorNamingToolSettings>();
	if (!ActorNamingToolSettings.IsValid()) return;

	CmdsRegister();

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhActorNamingToolSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(ActorNamingToolSettings.Get());

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
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SettingsProperty
				]
			]
			+ SSplitter::Slot().Value(0.6f)
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
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhActorNamingToolListItem>>)
						.ListItemsSource(&ListItems)
						.ClearSelectionOnClick(true)
						.SelectionMode(ESelectionMode::Multi)
						.OnGenerateRow(this, &SGdhActorNamingTool::OnGenerateRow)
						.HeaderRow(GetHeaderRow())
					]
				]
			]
		]
	];
}

void SGdhActorNamingTool::CmdsRegister()
{
	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCmds::Get().ScanActors,
		FExecuteAction::CreateRaw(this, &SGdhActorNamingTool::OnScanActors)
	);
	Cmds->MapAction(
		FGdhCmds::Get().OrganizeActors,
		FExecuteAction::CreateRaw(this, &SGdhActorNamingTool::OnOrganizeActors)
	);
	Cmds->MapAction(
		FGdhCmds::Get().RenameActors,
		FExecuteAction::CreateRaw(this, &SGdhActorNamingTool::OnRenameActors)
	);
	Cmds->MapAction(
		FGdhCmds::Get().UndoAction,
		FExecuteAction::CreateRaw(this, &SGdhActorNamingTool::OnUndo)
	);
}

void SGdhActorNamingTool::OnScanActors()
{
	UpdateListData();
	UpdateListView();
}

void SGdhActorNamingTool::OnOrganizeActors()
{
	UKismetSystemLibrary::BeginTransaction(TEXT("ActorNamingTool"), FText::FromString(TEXT("Actor Naming Tool: Organizing")), nullptr);

	for (const auto& Item : ListItems)
	{
		if (!Item.IsValid()) continue;
		if (!Item->Actor) continue;

		UKismetSystemLibrary::TransactObject(Item->Actor);
		Item->Actor->SetFolderPath(FName{*Item->FolderName});
	}

	UKismetSystemLibrary::EndTransaction();
}

void SGdhActorNamingTool::OnRenameActors()
{
	UKismetSystemLibrary::BeginTransaction(TEXT("ActorNamingTool"), FText::FromString(TEXT("Actor Naming Tool: Renaming")), nullptr);

	for (const auto& Item : ListItems)
	{
		if (!Item.IsValid()) continue;
		if (!Item->Actor) continue;

		UKismetSystemLibrary::TransactObject(Item->Actor);
		Item->Actor->SetActorLabel(Item->NewName, true);
	}

	UKismetSystemLibrary::EndTransaction();
}

void SGdhActorNamingTool::OnUndo()
{
	if (!GEditor) return;

	GEditor->UndoTransaction();
}

void SGdhActorNamingTool::UpdateListData()
{
	if (!ActorNamingToolSettings.IsValid()) return;
	if (!ActorNamingToolSettings->Mappings.LoadSynchronous()) return;

	TArray<FGdhActorAffixRow*> Rows;
	ActorNamingToolSettings->Mappings->GetAllRows<FGdhActorAffixRow>(TEXT(""), Rows);

	struct FGdhActorInfo
	{
		FString Prefix;
		FString Suffix;
		FString Folder;
	};

	TMap<FString, FGdhActorInfo> AffixMap;
	AffixMap.Reserve(Rows.Num());

	for (const auto& Row : Rows)
	{
		if (!Row) continue;
		if (!Row->bEnabled) continue;
		if (!Row->ActorClass.LoadSynchronous()) continue;

		FGdhActorInfo Info;
		Info.Prefix = Row->Prefix;
		Info.Suffix = Row->Suffix;
		Info.Folder = Row->Folder;

		AffixMap.Add(Row->ActorClass->GetName(), Info);
	}

	ListItems.Reset();
	const auto LevelActors = UEditorLevelLibrary::GetAllLevelActors();

	for (const auto& Actor : LevelActors)
	{
		if (!Actor) continue;

		UGdhActorNamingToolListItem* NewItem = NewObject<UGdhActorNamingToolListItem>();
		if (!NewItem) continue;

		const FString ActorClass = Actor->GetClass()->GetName();
		const FGdhActorInfo* Affix = AffixMap.Find(ActorClass);
		if (!Affix) continue;

		FString ActorBaseName = Actor->GetActorLabel().ToLower();
		ActorBaseName.RemoveFromStart(Affix->Prefix.ToLower());
		ActorBaseName.RemoveFromEnd(Affix->Suffix.ToLower());

		const FString Prefix = Affix->Prefix.IsEmpty() ? "" : UGdhLibString::ConvertNamingCase(Affix->Prefix, ActorNamingToolSettings->PrefixNamingCase) + TEXT("_");
		const FString Suffix = Affix->Suffix.IsEmpty() ? "" : TEXT("_") + UGdhLibString::ConvertNamingCase(Affix->Suffix, ActorNamingToolSettings->SuffixNamingCase);
		const FString ActorName = UGdhLibString::ConvertNamingCase(Actor->GetActorLabel(), ActorNamingToolSettings->ActorNamingCase);
		const FString NewName = Prefix + ActorName + Suffix;
		const FString FolderName = UGdhLibString::ConvertNamingCase(Affix->Folder, ActorNamingToolSettings->FolderNamingCase);

		NewItem->Prefix = Affix->Prefix;
		NewItem->Suffix = Affix->Suffix;
		NewItem->OldName = Actor->GetActorLabel();
		NewItem->NewName = NewName;
		NewItem->FolderName = FolderName;
		NewItem->Actor = Actor;

		ListItems.Add(NewItem);
	}
}

void SGdhActorNamingTool::UpdateListView() const
{
	if (!ListView) return;

	ListView->ClearSelection();
	ListView->ClearHighlightedItems();
	ListView->RebuildList();
}

TSharedRef<SWidget> SGdhActorNamingTool::CreateToolbarMain() const
{
	FToolBarBuilder ToolBarBuilder{Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhActorNamingToolMainToolbar");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().ScanActors);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().OrganizeActors);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().RenameActors);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().UndoAction);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SHeaderRow> SGdhActorNamingTool::GetHeaderRow()
{
	return
			SNew(SHeaderRow)
			+ SHeaderRow::Column(TEXT("Prefix"))
			.HAlignHeader(HAlign_Center)
			.VAlignHeader(VAlign_Center)
			.HeaderContentPadding(FMargin{5.0f})
			.FixedWidth(10.0f)
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
			.HeaderContentPadding(FMargin{5.0f})
			.FixedWidth(10.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Suffix")))
				.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
				.Font(FGdhStyles::GetFont("Light", 10.0f))
			]
			+ SHeaderRow::Column(TEXT("OldName"))
			.HAlignHeader(HAlign_Center)
			.VAlignHeader(VAlign_Center)
			.HeaderContentPadding(FMargin{5.0f})
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("OldName")))
				.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
				.Font(FGdhStyles::GetFont("Light", 10.0f))
			]
			+ SHeaderRow::Column(TEXT("NewName"))
			.HAlignHeader(HAlign_Center)
			.VAlignHeader(VAlign_Center)
			.HeaderContentPadding(FMargin{5.0f})
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("NewName")))
				.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
				.Font(FGdhStyles::GetFont("Light", 10.0f))
				.ToolTipText(FText::FromName(TEXT("Status")))
			]
			+ SHeaderRow::Column(TEXT("FolderName"))
			.HAlignHeader(HAlign_Center)
			.VAlignHeader(VAlign_Center)
			.HeaderContentPadding(FMargin{5.0f})
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Folder")))
				.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
				.Font(FGdhStyles::GetFont("Light", 10.0f))
				.ToolTipText(FText::FromName(TEXT("Status")))
			];
}

TSharedRef<ITableRow> SGdhActorNamingTool::OnGenerateRow(TWeakObjectPtr<UGdhActorNamingToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SGdhActorNamingToolListItem, OwnerTable).ListItem(Item);
}
