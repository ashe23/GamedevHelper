﻿// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/WorldOutlinerManager/GamedevHelperWorldOutlinerManagerWindow.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "GamedevHelperProjectSettings.h"
// Engine Headers
#include "EditorActorFolders.h"
#include "EditorLevelLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Editor/EditorEngine.h"
#include "Widgets/Layout/SScrollBox.h"

void SWorldOutlinerManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	Settings = GetMutableDefault<UGamedevHelperWorldOutlinerSettings>();

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
	SettingsProperty->SetObject(Settings);

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
				.OnClicked_Raw(this, &SWorldOutlinerManagerWindow::OnOrganizeBtnClicked)
				.ToolTipText(FText::FromString(TEXT("Organizes all actors in world outliner based on specified settings")))
				[
					SNew(STextBlock)
					.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Bold10"))
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
				.OnClicked_Static(&SWorldOutlinerManagerWindow::OnUndoBtnClicked)
				.ToolTipText(FText::FromString(TEXT("Undo last action. Same as pressing Ctrl + Z")))
				[
					SNew(STextBlock)
					.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Bold10"))
					.Text(FText::FromString(TEXT("Undo")))
				]
			]
			// + SVerticalBox::Slot()
			//   .Padding(FMargin{2.0f})
			//   .AutoHeight()
			// [
			// 	SNew(SButton)
			// 	.HAlign(HAlign_Center)
			// 	.VAlign(VAlign_Center)
			// 	.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#EF5350"))})
			// 	.ContentPadding(FMargin{5})
			// 	.OnClicked_Static(&SWorldOutlinerManagerWindow::OnRemoveEmptyFoldersBtnClicked)
			// 	.ToolTipText(FText::FromString(TEXT("Removes all empty folders in world outliner")))
			// 	[
			// 		SNew(STextBlock)
			// 		.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Bold10"))
			// 		.Text(FText::FromString(TEXT("Remove empty folders")))
			// 	]
			// ]
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

FReply SWorldOutlinerManagerWindow::OnOrganizeBtnClicked() const
{
	UKismetSystemLibrary::BeginTransaction(TEXT("WorldOutlinerOrganizer"),
	                                       FText::FromString(TEXT("Organizes all actors in world outliner")),
	                                       nullptr);

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem) return FReply::Handled();
	
	const auto LevelActors = EditorActorSubsystem->GetAllLevelActors();
	const auto Mappings = Settings->Mappings;

	for (const auto LevelActor : LevelActors)
	{
		FName FolderName = Settings->GetFolderNameByActor(LevelActor);

		if (Settings->bMoveToUnsorted && FolderName.IsNone() && !Settings->UnsortedFolderName.IsEmpty())
		{
			FolderName = FName{Settings->UnsortedFolderName};
		}

		if (!FolderName.IsNone())
		{
			UKismetSystemLibrary::TransactObject(LevelActor);
			LevelActor->SetFolderPath(FolderName);
		}
	}

	UKismetSystemLibrary::EndTransaction();

	return FReply::Handled();
}

FReply SWorldOutlinerManagerWindow::OnUndoBtnClicked()
{
	if (GEditor)
	{
		GEditor->UndoTransaction();
	}

	return FReply::Handled();
}

FReply SWorldOutlinerManagerWindow::OnRemoveEmptyFoldersBtnClicked()
{
	// if (!GEditor) return FReply::Handled();
	//
	// const TObjectPtr<UEditorActorSubsystem> EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	// if (!EditorActorSubsystem) return FReply::Handled();
	//
	// const FWorldContext& WorldContext = GEditor->GetEditorWorldContext(false);
	// if (!WorldContext.World()) return FReply::Handled();
	//
	// TArray<FFolder> LevelFolders;
	// TArray<FFolder> RootObjectFolders;
	// FActorFolders::Get().ForEachFolder(*WorldContext.World(),[&LevelFolders, &RootObjectFolders](const FFolder& Folder)
	// {
	// 	if (Folder.HasRootObject())
	// 	{
	// 		RootObjectFolders.Add(Folder);
	// 		return true;
	// 	}
	// 	
	// 	LevelFolders.Add(Folder);
	// 	return true;
	// });

	// todo:ashe23 engine clears all empty folders automatically, for now we just hide remove button and do it later
	
	// const auto LevelFolders = FActorFolders::Get().GetFolderPropertiesForWorld(*World);
	// const auto LevelFolders = FActorFolders::GetDefaultFolderName(*World, FFolder{TEXT("")});
	// const auto LevelActors = EditorActorSubsystem->GetAllLevelActors();
	//
	// TSet<FString> UsedFolders;
	// UsedFolders.Reserve(LevelFolders.Num());
	//
	// for (const auto& LevelActor : LevelActors)
	// {
	// 	const auto ActorFolderPath = LevelActor->GetFolderPath();
	//
	// 	// add also parent folders hierarchy
	// 	const TArray<FString> ParentFoldersParts = UKismetStringLibrary::ParseIntoArray(ActorFolderPath.ToString(), TEXT("/"), true);
	// 	if (ParentFoldersParts.Num() > 0)
	// 	{
	// 		FString CurrentPath;
	// 		CurrentPath.Append(ParentFoldersParts[0]);
	// 		UsedFolders.Add(CurrentPath);
	//
	// 		for (int32 i = 1; i < ParentFoldersParts.Num(); ++i)
	// 		{
	// 			CurrentPath.Append(TEXT("/") + ParentFoldersParts[i]);
	// 			UsedFolders.Add(CurrentPath);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		UsedFolders.Add(ActorFolderPath.ToString());
	// 	}
	// }
	//
	// TSet<FName> EmptyFolders;
	// EmptyFolders.Reserve(LevelFolders.Num());
	//
	// for (const auto& LevelFolder : LevelFolders)
	// {
	// 	if (!UsedFolders.Contains(LevelFolder.Key.ToString()))
	// 	{
	// 		EmptyFolders.Add(LevelFolder.Key);
	// 	}
	// }
	//
	// for (const auto& EmptyFolder : EmptyFolders)
	// {
	// 	FActorFolders::Get().DeleteFolder(*World, EmptyFolder);
	// }

	return FReply::Handled();
}
