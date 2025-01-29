// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FGdhVideoPipelineActions : public FAssetTypeActions_Base
{
  public:
	FGdhVideoPipelineActions(const EAssetTypeCategories::Type& InAssetTypeCategory);

	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
private:
	EAssetTypeCategories::Type Category;
};
