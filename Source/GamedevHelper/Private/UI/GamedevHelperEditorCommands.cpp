// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GamedevHelperEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

void FGamedevHelperEditorCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_AssetNamingManagerWindow, "Asset Naming Manager", "Open Asset Naming Manager Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
