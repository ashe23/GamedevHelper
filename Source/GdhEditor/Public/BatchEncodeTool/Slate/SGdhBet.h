// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "Widgets/SCompoundWidget.h"

class SGdhBet final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhBet) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	void OnEncodePresetRefresh();
	void OnEncodePresetCreate();

	void OnPathSelected(const FString& InPath);
	void FilterUpdate();

	TSharedRef<SWidget> CreateToolbarPresets() const;
	// TSharedRef<SWidget> CreateToolbarQueue() const;

	FString PathCurrent;
	FARFilter Filter;
	TSharedPtr<FUICommandList> Cmds;
	FSetARFilterDelegate DelegateFilter;
};
