// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GamedevHelperEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

void FGamedevHelperEditorCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_AssetNamingManagerWindow, "Asset Naming Manager", "Open Asset Naming Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_WorldOutlinerManagerWindow, "World Outlinear Manager", "Open World Outlinear Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_AssetNamingManagerRenameSelected, "Renamed Selected", "Rename only selected assets", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_AssetNamingManagerOpenAsset, "Open Asset", "Open selected asset", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
