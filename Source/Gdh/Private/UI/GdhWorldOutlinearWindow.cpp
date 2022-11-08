// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhWorldOutlinearWindow.h"

#include "GdhStyles.h"
#include "Settings/GdhWorldOutlinearSettings.h"
// Engine Headers
#include "WorldFolders.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhWorldOutlinearWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	WorldOutlinearSettings = GetMutableDefault<UGdhWorldOutlinearSettings>();

	check(WorldOutlinearSettings.Get());

	FDetailsViewArgs ViewArgs;
	ViewArgs.bUpdatesFromSelection = false;
	ViewArgs.bLockable = false;
	ViewArgs.bShowScrollBar = true;
	ViewArgs.bAllowSearch = true;
	ViewArgs.bShowOptions = false;
	ViewArgs.bAllowFavoriteSystem = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.ViewIdentifier = "WorldOutlinerManagerSettings";
	const TSharedPtr<IDetailsView> SettingsProperty = PropertyEditor.CreateDetailView(ViewArgs);
	SettingsProperty->SetObject(WorldOutlinearSettings.Get());

	ChildSlot
	[
		SNew(SSplitter)
		.Style(&FAppStyle::Get().GetWidgetStyle<FSplitterStyle>("Splitter"))
		.Orientation(Orient_Horizontal)
		.PhysicalSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(0.3f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			  .Padding(FMargin{2.0f})
			  .AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
				.ContentPadding(FMargin{5})
				.OnClicked_Raw(this, &SGdhWorldOutlinearWindow::OnOrganizeBtnClicked)
				.ToolTipText(FText::FromString(TEXT("Organizes all actors in world outliner based on specified settings")))
				[
					SNew(STextBlock)
					.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Bold10"))
					.Text(FText::FromString(TEXT("Organize")))
				]
			]
			+ SVerticalBox::Slot()
			  .Padding(FMargin{2.0f})
			  .AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#FFEE58"))})
				.ContentPadding(FMargin{5})
				.OnClicked_Static(&SGdhWorldOutlinearWindow::OnUndoBtnClicked)
				.ToolTipText(FText::FromString(TEXT("Undo last action. Same as pressing Ctrl + Z")))
				[
					SNew(STextBlock)
					.Font(FGdhStyles::Get().GetFontStyle("GamedevHelper.Font.Bold10"))
					.Text(FText::FromString(TEXT("Undo")))
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
				[
					SettingsProperty.ToSharedRef()
				]
			]
		]
	];
}

FReply SGdhWorldOutlinearWindow::OnOrganizeBtnClicked() const
{
	UKismetSystemLibrary::BeginTransaction(TEXT("WorldOutlinerOrganizer"),
	                                       FText::FromString(TEXT("Organizes all actors in world outliner")),
	                                       nullptr);

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem) return FReply::Handled();

	const auto LevelActors = EditorActorSubsystem->GetAllLevelActors();

	if (WorldOutlinearSettings->bOrganizeByType)
	{
		for (const auto& LevelActor : LevelActors)
		{
			// todo:ashe23 if blueprint remove _C prefix
			FString ActorClassName = LevelActor->GetClass()->GetName();
			ActorClassName.RemoveFromEnd(TEXT("_C"));
			
			LevelActor->SetFolderPath(FName{*ActorClassName});
		}
	}
	else
	{
		const auto Mappings = WorldOutlinearSettings->Mappings;

		for (const auto& LevelActor : LevelActors)
		{
			FName FolderName = GetFolderNameByActor(LevelActor);

			if (WorldOutlinearSettings->bMoveToUnsorted && FolderName.IsNone() && !WorldOutlinearSettings->UnsortedFolderName.IsEmpty())
			{
				FolderName = FName{WorldOutlinearSettings->UnsortedFolderName};
			}

			if (!FolderName.IsNone())
			{
				UKismetSystemLibrary::TransactObject(LevelActor);
				LevelActor->SetFolderPath(FolderName);
			}
		}
	}


	UKismetSystemLibrary::EndTransaction();

	return FReply::Handled();
}

FReply SGdhWorldOutlinearWindow::OnUndoBtnClicked()
{
	if (GEditor)
	{
		GEditor->UndoTransaction();
	}

	return FReply::Handled();
}

FName SGdhWorldOutlinearWindow::GetFolderNameByActor(const AActor* Actor) const
{
	if (!Actor)
	{
		return NAME_None;
	}

	const UClass* OtherActorClass = Actor->GetClass();
	if (!OtherActorClass)
	{
		return NAME_None;
	}

	for (const auto& Mapping : WorldOutlinearSettings->Mappings)
	{
		const UClass* MappingClass = Mapping.Key;
		if (!MappingClass) continue;

		if (MappingClass == OtherActorClass || OtherActorClass->IsChildOf(MappingClass))
		{
			return Mapping.Value;
		}
	}

	return NAME_None;
}
