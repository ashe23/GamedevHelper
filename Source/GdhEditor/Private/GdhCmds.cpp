// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCmds.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCmds::RegisterCommands()
{
	UI_COMMAND(RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenAssetNamingTool, "Asset Naming Tool", "Open Asset Naming Tool Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ScanAssets, "Scan assets", "Scan assets", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RenameAssets, "Rename assets", "Rename assets", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
