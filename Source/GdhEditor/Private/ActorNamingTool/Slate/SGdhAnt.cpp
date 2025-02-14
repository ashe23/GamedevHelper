// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ActorNamingTool/Slate/SGdhAnt.h"
#include "ActorNamingTool/Slate/SGdhAntListItem.h"
#include "ActorNamingTool/GdhAntSettings.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "GdhStructs.h"
#include "GdhLibEditor.h"
#include "GdhLibString.h"
#include "ClassIconFinder.h"
#include "EditorLevelLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhAnt::Construct(const FArguments& InArgs) {

	AntSettings = GetMutableDefault<UGdhAntSettings>();
	if (!AntSettings.IsValid()) return;

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
	SettingsProperty->SetObject(AntSettings.Get());

	UpdateListData();
	UpdateListView();

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
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhAntListItem>>)
						.ListItemsSource(&ListItems)
						.ClearSelectionOnClick(true)
						.SelectionMode(ESelectionMode::Multi)
						.OnGenerateRow(this, &SGdhAnt::OnGenerateRow)
						.HeaderRow(GetHeaderRow())
					]
				]
			]
		]
	];
	// clang-format on
}
void SGdhAnt::CmdsRegister() {
	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(FGdhCmds::Get().AntScanActors, FExecuteAction::CreateRaw(this, &SGdhAnt::OnScanActors));
	Cmds->MapAction(FGdhCmds::Get().AntOrganizeActors, FExecuteAction::CreateRaw(this, &SGdhAnt::OnOrganizeActors));
	Cmds->MapAction(FGdhCmds::Get().AntRenameActors, FExecuteAction::CreateRaw(this, &SGdhAnt::OnRenameActors));
	Cmds->MapAction(FGdhCmds::Get().AntUndoAction, FExecuteAction::CreateRaw(this, &SGdhAnt::OnUndo));
}
void SGdhAnt::OnScanActors() {
	UpdateListData();
	UpdateListView();
}
void SGdhAnt::OnOrganizeActors() {
	UKismetSystemLibrary::BeginTransaction(TEXT("ActorNamingTool"), FText::FromString(TEXT("Actor Naming Tool: Organizing")), nullptr);

	for (const auto& Item : ListItems) {
		if (!Item.IsValid()) continue;
		if (!Item->Actor) continue;

		UKismetSystemLibrary::TransactObject(Item->Actor);
		Item->Actor->SetFolderPath(FName {*Item->FolderName});
	}

	UKismetSystemLibrary::EndTransaction();
}
void SGdhAnt::OnRenameActors() {
	UKismetSystemLibrary::BeginTransaction(TEXT("ActorNamingTool"), FText::FromString(TEXT("Actor Naming Tool: Renaming")), nullptr);

	for (const auto& Item : ListItems) {
		if (!Item.IsValid()) continue;
		if (!Item->Actor) continue;

		UKismetSystemLibrary::TransactObject(Item->Actor);
		Item->Actor->SetActorLabel(Item->NewName, true);
	}

	UKismetSystemLibrary::EndTransaction();
}
void SGdhAnt::OnUndo() {
	if (!GEditor) return;

	GEditor->UndoTransaction();
}
void SGdhAnt::UpdateListData() {
	if (!AntSettings.IsValid()) return;
	if (!AntSettings->Mappings.LoadSynchronous()) return;

	TArray<FGdhActorAffixRow*> Rows;
	AntSettings->Mappings->GetAllRows<FGdhActorAffixRow>(TEXT(""), Rows);

	struct FGdhActorInfo
	{
		FString Prefix;
		FString Suffix;
		FString Folder;
	};

	TMap<FString, FGdhActorInfo> AffixMap;
	AffixMap.Reserve(Rows.Num());

	for (const auto& Row : Rows) {
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

	for (const auto& Actor : LevelActors) {
		if (!Actor) continue;

		UGdhAntListItem* NewItem = NewObject<UGdhAntListItem>();
		if (!NewItem) continue;

		const FString ActorClass = Actor->GetClass()->GetName();
		const FGdhActorInfo* Affix = AffixMap.Find(ActorClass);
		if (!Affix) continue;

		FString ActorBaseName = Actor->GetActorLabel().ToLower();
		ActorBaseName.RemoveFromStart(Affix->Prefix.ToLower());
		ActorBaseName.RemoveFromEnd(Affix->Suffix.ToLower());

		const FString Prefix =
			Affix->Prefix.IsEmpty() ? "" : UGdhLibString::ConvertNamingCase(Affix->Prefix, AntSettings->PrefixNamingCase) + TEXT("_");
		const FString Suffix =
			Affix->Suffix.IsEmpty() ? "" : TEXT("_") + UGdhLibString::ConvertNamingCase(Affix->Suffix, AntSettings->SuffixNamingCase);
		const FString ActorName = UGdhLibString::ConvertNamingCase(Actor->GetActorLabel(), AntSettings->ActorNamingCase);
		const FString NewName = Prefix + ActorName + Suffix;

		if (NewName.Equals(Actor->GetActorLabel(), ESearchCase::CaseSensitive)) continue;

		const FString FolderName = UGdhLibString::ConvertNamingCase(Affix->Folder, AntSettings->FolderNamingCase);

		NewItem->Prefix = Affix->Prefix;
		NewItem->Suffix = Affix->Suffix;
		NewItem->OldName = Actor->GetActorLabel();
		NewItem->NewName = NewName;
		NewItem->FolderName = FolderName;
		NewItem->Actor = Actor;
		NewItem->ActorIcon = FClassIconFinder::FindIconForActor(Actor);

		ListItems.Add(NewItem);
	}
}
void SGdhAnt::UpdateListView() const {
	if (!ListView) return;

	ListView->ClearSelection();
	ListView->ClearHighlightedItems();
	ListView->RebuildList();
}
TSharedRef<SWidget> SGdhAnt::CreateToolbarMain() const {
	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhActorNamingToolMainToolbar");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().AntScanActors);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().AntOrganizeActors);
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().AntRenameActors);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().AntUndoAction);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}
TSharedRef<SHeaderRow> SGdhAnt::GetHeaderRow() {

	// clang-format off
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(TEXT("Preview"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})

		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Preview")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
			.ToolTipText(FText::FromName(TEXT("Actor rename preview")))
		]
		+ SHeaderRow::Column(TEXT("FolderName"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		.FillWidth(0.2f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Folder")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Prefix"))
		.FillWidth(0.1f)
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Prefix")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Suffix"))
		.FillWidth(0.1f)
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Suffix")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
			.ToolTipText(FText::FromName(TEXT("Status")))
		];

	// clang-format on
}
TSharedRef<ITableRow> SGdhAnt::OnGenerateRow(TWeakObjectPtr<UGdhAntListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGdhAntListItem, OwnerTable).ListItem(Item);
}
