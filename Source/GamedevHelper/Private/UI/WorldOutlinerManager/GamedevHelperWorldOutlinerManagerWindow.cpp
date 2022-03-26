// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/WorldOutlinerManager/GamedevHelperWorldOutlinerManagerWindow.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "GamedevHelperProjectSettings.h"
// Engine Headers
#include "EditorLevelLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#42A5F5"))})
				.ContentPadding(FMargin{0})
				.OnClicked_Raw(this, &SWorldOutlinerManagerWindow::OnOrganizeBtnClicked)
				[
					SNew(STextBlock)
					.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Bold20"))
					.Text(FText::FromString(TEXT("Organize")))
				]
			]
			+ SVerticalBox::Slot()
			  .Padding(FMargin{10.0f})
			  .AutoHeight()
			[
				SettingsProperty.ToSharedRef()
			]
		]
	];
}

FReply SWorldOutlinerManagerWindow::OnOrganizeBtnClicked() const
{
	UKismetSystemLibrary::BeginTransaction(TEXT("WorldOutlinerOrganizer"),
	                                       FText::FromString(TEXT("Organizes all actors in world outliner")),
	                                       nullptr);

	const auto LevelActors = UEditorLevelLibrary::GetAllLevelActors();
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
			// todo:ashe23 rename actor if settings enabled
			LevelActor->SetFolderPath(FolderName);
		}
	}

	UKismetSystemLibrary::EndTransaction();

	return FReply::Handled();
}
