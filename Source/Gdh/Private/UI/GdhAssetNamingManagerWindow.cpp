// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhAssetNamingManagerWindow.h"

void SGdhAssetNamingManagerWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromName(TEXT("AAA")))
		]
	];
}
