// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGdhActorNamingTool final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhActorNamingTool) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
