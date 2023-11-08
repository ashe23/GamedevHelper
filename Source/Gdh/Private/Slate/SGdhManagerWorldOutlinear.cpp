// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhManagerWorldOutlinear.h"
#include "Settings/GdhWorldOutlinearSettings.h"
#include "GdhSubsystem.h"
#include "GdhCmds.h"
// Engine Headers
#include "EditorActorFolders.h"
#include "EditorLevelLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Layout/SScrollBox.h"

void SGdhManagerWorldOutlinear::Construct(const FArguments& InArgs)
{
	WorldOutlinearSettings = GetMutableDefault<UGdhWorldOutlinearSettings>();
	if (!WorldOutlinearSettings.IsValid()) return;

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCommands::Get().Cmd_OrganizeActors,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				UKismetSystemLibrary::BeginTransaction(
					TEXT("WorldOutlinerOrganizer"),
					FText::FromString(TEXT("Organizes all actors in world outliner")),
					nullptr);

				const TArray<AActor*> ActorsAll = UEditorLevelLibrary::GetAllLevelActors();

				for (const auto& Actor : ActorsAll)
				{
					if (!Actor) continue;

					const UClass* ActorClass = Actor->GetClass();
					if (!ActorClass) continue;

					const FString* MappingInfo = WorldOutlinearSettings->Mappings.Find(ActorClass);
					const bool CanMoveToUnsortedFolder = WorldOutlinearSettings->bMoveToUnsorted && !WorldOutlinearSettings->UnsortedFolderName.IsEmpty();
					const bool ShouldMoveToUnsortedFolder = !MappingInfo || MappingInfo->IsEmpty();

					if (ShouldMoveToUnsortedFolder)
					{
						if (CanMoveToUnsortedFolder)
						{
							UKismetSystemLibrary::TransactObject(Actor);
							Actor->SetFolderPath(FName{*WorldOutlinearSettings->UnsortedFolderName});
						}
					}
					else
					{
						UKismetSystemLibrary::TransactObject(Actor);
						Actor->SetFolderPath(FName{*MappingInfo});
					}
				}

				UKismetSystemLibrary::EndTransaction();
			})
		)
	);

	Cmds->MapAction(
		FGdhCommands::Get().Cmd_Undo,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (!GEditor) return;

				GEditor->UndoTransaction();
			})
		)
	);

	Cmds->MapAction(
		FGdhCommands::Get().Cmd_DeleteEmptyFolders,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				UWorld* EditorWorld = UEditorLevelLibrary::GetEditorWorld();
				if (!EditorWorld) return;

				UKismetSystemLibrary::BeginTransaction(
					TEXT("WorldOutlinerOrganizer"),
					FText::FromString(TEXT("Removing empty folders in outliner")),
					nullptr);

				const auto LevelFolders = FActorFolders::Get().GetFolderPropertiesForWorld(*EditorWorld);
				const auto LevelActors = UEditorLevelLibrary::GetAllLevelActors();

				TSet<FString> UsedFolders;
				UsedFolders.Reserve(LevelFolders.Num());

				for (const auto& LevelActor : LevelActors)
				{
					const auto ActorFolderPath = LevelActor->GetFolderPath();

					// add also parent folders hierarchy
					const TArray<FString> ParentFoldersParts = UKismetStringLibrary::ParseIntoArray(ActorFolderPath.ToString(), TEXT("/"), true);
					if (ParentFoldersParts.Num() > 0)
					{
						FString CurrentPath;
						CurrentPath.Append(ParentFoldersParts[0]);
						UsedFolders.Add(CurrentPath);

						for (int32 i = 1; i < ParentFoldersParts.Num(); ++i)
						{
							CurrentPath.Append(TEXT("/") + ParentFoldersParts[i]);
							UsedFolders.Add(CurrentPath);
						}
					}
					else
					{
						UsedFolders.Add(ActorFolderPath.ToString());
					}
				}

				TSet<FName> EmptyFolders;
				EmptyFolders.Reserve(LevelFolders.Num());

				for (const auto& LevelFolder : LevelFolders)
				{
					if (!UsedFolders.Contains(LevelFolder.Key.ToString()))
					{
						EmptyFolders.Add(LevelFolder.Key);
					}
				}

				for (const auto& EmptyFolder : EmptyFolders)
				{
					UKismetSystemLibrary::TransactObject(EditorWorld);
					FActorFolders::Get().DeleteFolder(*EditorWorld, EmptyFolder);
				}

				UKismetSystemLibrary::EndTransaction();
			})
		)
	);

	FDetailsViewArgs ViewArgs;
	ViewArgs.bUpdatesFromSelection = false;
	ViewArgs.bLockable = false;
	ViewArgs.bShowScrollBar = true;
	ViewArgs.bAllowSearch = false;
	ViewArgs.bShowOptions = false;
	ViewArgs.bAllowFavoriteSystem = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.ViewIdentifier = "GdhWorldOutlinearSettings";

	const TSharedPtr<IDetailsView> SettingsProperty = UGdhSubsystem::GetModulePropertyEditor().CreateDetailView(ViewArgs);
	SettingsProperty->SetObject(WorldOutlinearSettings.Get());

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(3.0f)
		[
			CreateToolbarMain()
		]
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(3.0f)
		[
			SNew(SBox)
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+ SScrollBox::Slot()
				[
					SettingsProperty.ToSharedRef()
				]
			]
		]
	];
}

TSharedRef<SWidget> SGdhManagerWorldOutlinear::CreateToolbarMain() const
{
	FToolBarBuilder ToolBarBuilder{Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhWorldOutlinearSectionMain");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().Cmd_OrganizeActors);
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().Cmd_Undo);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().Cmd_DeleteEmptyFolders);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}
