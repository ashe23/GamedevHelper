// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhBetEncodePreset.h"
#include "Toolkits/AssetEditorToolkit.h"

class FGdhBetEncodePresetEdToolkit final : public FAssetEditorToolkit
{
public:

	void InitEditor(const TArray<UObject*>& InObjects);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:

	TSharedRef<SDockTab> OnTabSpawn(const FSpawnTabArgs& Args);

	UGdhBetEncodePreset* EncodePreset = nullptr;
};
