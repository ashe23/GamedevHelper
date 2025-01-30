// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdhEditor, Log, All);

class FGdhRenderListActions;

class FGdhEditorModule final : public IModuleInterface
{
  public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

  private:

	TSharedPtr<FGdhRenderListActions> RenderListActions;
};
