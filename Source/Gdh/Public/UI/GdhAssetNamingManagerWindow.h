// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/GdhAssetNamingConventionSettings.h"
#include "Widgets/SCompoundWidget.h"

class SGdhAssetNamingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhAssetNamingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	TWeakObjectPtr<UGdhAssetNamingConventionSettings> AssetNamingConventionSettings;
};