// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGdhEditor"

void FGdhEditorCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(Cmd_AssetNamingManagerWindow, "Asset Naming Manager", "Open Asset Naming Manager Window", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(Cmd_WorldOutlinerManagerWindow, "World Outlinear Manager", "Open World Outlinear Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenRenderingManagerWindow, "Rendering Manager", "Open Rendering Manager Window", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(Cmd_AssetNamingManagerRenameSelected, "Renamed Selected", "Rename only selected assets", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(Cmd_AssetNamingManagerOpenAsset, "Open Asset", "Open selected asset", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(Cmd_RenderingManagerRemoveRenderedImages, "Remove Rendered Images", "Removes rendered images for selected job", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(Cmd_RenderingManagerRemoveRenderedVideo, "Remove Rendered Video", "Removes rendered video file for selected job", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
