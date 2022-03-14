// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SProjectOrganizerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SProjectOrganizerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
};