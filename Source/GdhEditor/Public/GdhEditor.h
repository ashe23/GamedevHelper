// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdhEditor, Log, All);

class FGdhNamingPolicyAssetActions;

class FGdhEditor final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;

	static EAssetTypeCategories::Type NamingPolicyAssetCategory;

private:
	TSharedPtr<FGdhNamingPolicyAssetActions> NamingPolicyAssetActions;
};
