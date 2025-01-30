// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhRenderList.h"
#include "Toolkits/AssetEditorToolkit.h"

class FGdhRenderListEdToolkit final : public FAssetEditorToolkit
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

	TSharedRef<SDockTab> OnTabSpawnSequences(const FSpawnTabArgs& Args);
	// TSharedRef<SDockTab> OnTabSpawnDetails(const FSpawnTabArgs& Args);

	UGdhRenderList* RenderList = nullptr;
};
