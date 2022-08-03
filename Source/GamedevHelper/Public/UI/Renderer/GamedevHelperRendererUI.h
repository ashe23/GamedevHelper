// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGamedevHelperRendererSettings;

class SGamedevHelperRendererUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRendererUI) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	UGamedevHelperRendererSettings* RendererSettings = nullptr;

	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
};