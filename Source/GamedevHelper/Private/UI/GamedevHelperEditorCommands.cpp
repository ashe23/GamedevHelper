// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GamedevHelperEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

void FGamedevHelperEditorCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_ProjectOrganizerWindow, "Project Organizer", "Open Project Organizer Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
