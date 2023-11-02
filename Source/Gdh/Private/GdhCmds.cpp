// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCmds.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
