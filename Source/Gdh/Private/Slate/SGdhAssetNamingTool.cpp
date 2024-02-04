// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhAssetNamingTool.h"

void SGdhAssetNamingTool::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STextBlock)
		.Text(FText::FromName(TEXT("AAA")))
	];
}
