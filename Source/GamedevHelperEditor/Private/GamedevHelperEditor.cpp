// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperEditor.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

class FGamedevHelperEditor : public IGamedevHelperEditor
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FGamedevHelperEditor::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGamedevHelperEditor::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGamedevHelperEditor, GamedevHelperEditor)
DEFINE_LOG_CATEGORY(LogGamedevHelperEditor);