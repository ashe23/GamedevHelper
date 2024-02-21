// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorModule.h"

DEFINE_LOG_CATEGORY(LogGdhEditor);

void FGdhEditorModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FGdhEditorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_MODULE(FGdhEditorModule, GdhEditorModule)
