// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCmds.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCmds::RegisterCommands() {
	// clang-format off
	// Main Menu
	UI_COMMAND(RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenActorNamingTool, "Actor Naming Tool", "Open Actor Naming Tool Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenBatchEncodeTool, "Batch Encode Tool", "Open Batch Encode Tool Editor", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(OpenAssetNamingTool, "Asset Naming Tool(ANT)", "Open Asset Naming Tool Editor", EUserInterfaceActionType::Button, FInputChord());

	// Asset Naming Tool
	// UI_COMMAND(ScanAssets, "Refresh", "Scan assets and refresh list", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(RenameAssets, "Rename", "Rename assets", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(ClearSelection, "Clear Selection", "Clear selection", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(EditMode, "Inline Edit", "Enable this to rename assets inplace", EUserInterfaceActionType::ToggleButton, FInputChord());
	// UI_COMMAND(ApplyChanges, "Apply Changes", "Apply all modifications", EUserInterfaceActionType::ToggleButton, FInputChord());
	// UI_COMMAND(UndoChanges, "Undo Changes", "Undo all modifications", EUserInterfaceActionType::ToggleButton, FInputChord());

	// Actor Naming Tool
	UI_COMMAND(AntScanActors, "Refresh", "Scan actors and refresh list", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AntOrganizeActors, "Organize Actors", "Organize actors in world outliner", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AntRenameActors, "Rename Actors", "Rename actors in world outliner", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AntUndoAction, "Undo", "Undo last action", EUserInterfaceActionType::Button, FInputChord());

	// Batch Encode Tool
	UI_COMMAND(BetRefresh, "Refresh", "Refresh", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetProcess, "Process", "Start Rendering and Encoding process", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetRenderListRefresh, "Refresh", "Refresh List", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetRenderListRemove, "Remove", "Remove Selected sequences from list", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetEncodePresetRefresh, "Refresh", "Refresh encode preset lists", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetEncodePresetCreate, "Create", "Create new encode preset", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetEncodePresetDuplicate, "Duplicate", "Duplicate existing encode preset", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetEncodePresetRename, "Rename", "Rename existing encode preset", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(BetEncodePresetRemove, "Remove", "Remove existing encode preset", EUserInterfaceActionType::Button, FInputChord());
	// clang-format on
}

#undef LOCTEXT_NAMESPACE
