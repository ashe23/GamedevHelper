// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FGdhBetEncodePresetActions final : public FAssetTypeActions_Base
{
public:

	FGdhBetEncodePresetActions(const EAssetTypeCategories::Type& InAssetTypeCategory);

	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor
	) override;

private:

	EAssetTypeCategories::Type Category;
};
