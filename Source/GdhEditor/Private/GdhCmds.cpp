// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCmds.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCmds::RegisterCommands()
{
	UI_COMMAND(RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenAssetNamingTool, "Asset Naming Tool", "Open Asset Naming Tool Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ScanAssets, "Refresh", "Scan assets and refresh list", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RenameAssets, "Rename", "Rename assets", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ClearSelection, "Clear Selection", "Clear selection", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(EditMode, "Inline Edit", "Enable this to rename assets inplace", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(ApplyChanges, "Apply Changes", "Apply all modifications", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(UndoChanges, "Undo Changes", "Undo all modifications", EUserInterfaceActionType::ToggleButton, FInputChord());
}

#undef LOCTEXT_NAMESPACE
