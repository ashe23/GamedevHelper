// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SAssetNamingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetNamingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SAssetNamingManagerWindow() override;
private:
	void InitTabs();
	TSharedRef<SDockTab> OnTabAssetNamingSpawn(const FSpawnTabArgs& SpawnTabArgs) const;
	TSharedRef<SDockTab> OnTabFolderStructureSpawn(const FSpawnTabArgs& SpawnTabArgs) const;

	TSharedPtr<FTabManager> TabManager;
	TSharedPtr<FTabManager::FLayout> TabLayout;
};