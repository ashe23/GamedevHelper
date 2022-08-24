// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhMovieRenderSettings;
class UGdhSubsystem;

class SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
private:
	int32 GetActiveWidgetIndex() const;
	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
	
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhMovieRenderSettings* MovieRenderSettings = nullptr;
	UGdhSubsystem* GdhSubsystem = nullptr;
	const int32 WidgetIndexRenderingManagerWorking = 0;
	const int32 WidgetIndexRenderingManagerUI = 1;
};