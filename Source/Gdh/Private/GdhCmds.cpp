// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCmds.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenAssetNamingManager, "Asset Naming Manager", "Open Asset Naming Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenWorldOutlinearManager, "World Outlinear Manager", "Open World Outlinear Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenMrqBatchRenderingManager, "MRQ Rendering Manager", "Open MovieRenderQueue Batch Rendering Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_ScanAssets, "Scan Assets", "Scan project assets", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_RenameAssets, "Rename Assets", "Rename all assets by naming convention", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_ClearSelection, "Clear Selection", "Clear any selection", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OrganizeActors, "Organize Actors", "Organize Actors in World Outliner based on settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_Undo, "Undo", "Undo last action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_DeleteEmptyFolders, "Delete Empty Folders", "Delete all empty folders from world outliner", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
