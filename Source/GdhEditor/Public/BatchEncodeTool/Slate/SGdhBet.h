// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGdhBet final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhBet) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
