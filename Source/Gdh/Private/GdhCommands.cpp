// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCommands.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenWindowRenderingManager, "Rendering Manager", "Open Rendering Manager Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
