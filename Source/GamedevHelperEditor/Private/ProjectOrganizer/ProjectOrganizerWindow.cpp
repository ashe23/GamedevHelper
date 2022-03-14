// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectOrganizer/ProjectOrganizerWindow.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

void SProjectOrganizerWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("ProjectOrganizerWindow")))
		]
	];
}

#undef LOCTEXT_NAMESPACE