// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdhEditor, Log, All);

class FGdhAssetNamingSettingsActions;

class FGdhEditor final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;

private:
	TSharedPtr<FGdhAssetNamingSettingsActions> AssetNamingSettingsActions;
};
