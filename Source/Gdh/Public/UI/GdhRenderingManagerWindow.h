// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhMovieRenderSettings;

class SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	int32 GetActiveWidgetIndex() const;
private:
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhMovieRenderSettings* MovieRenderSettings = nullptr;
	const int32 WidgetIndexRenderingManagerWorking = 0;
	const int32 WidgetIndexRenderingManagerUI = 1;
};