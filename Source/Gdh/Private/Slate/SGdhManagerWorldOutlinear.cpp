// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhManagerWorldOutlinear.h"

void SGdhManagerWorldOutlinear::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STextBlock).Text(FText::FromString(TEXT("World Outlinear Settings")))
	];
}
