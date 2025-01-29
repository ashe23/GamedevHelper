// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "GdhSettingsEncoderAssetActions.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdhEditor, Log, All);

class FGdhSettingsEncoderAssetActions;

class FGdhEditorModule final : public IModuleInterface
{
  public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

  private:

	TSharedPtr<FGdhSettingsEncoderAssetActions> SettingsEncoderAssetActions;
};
