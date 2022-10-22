// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhCommands.h"

#define LOCTEXT_NAMESPACE "FGdh"

void FGdhCommands::RegisterCommands()
{
	UI_COMMAND(Cmd_RestartEditor, "Restart", "Restart Editor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenWindowRenderingManager, "Rendering Manager", "Open Rendering Manager Window", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenImageDir, "Open Image Directory", "Open selected LevelSequence rendered images directory", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_OpenVideoDir, "Open Video Directory", "Open selected LevelSequence encoded video directory", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_PlayVideoFile, "Play Video", "Plays selected LevelSequence encoded video in external video player", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_RemoveRenderedImages, "Remove rendered images", "Removes all existing rendered images for selected LevelSequence", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_RemoveEncodedVideo, "Remove encoded video", "Removes encoded video file for selected LevelSequence", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_RenderSelected, "Render Selected", "Render images for selected job", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(Cmd_EncodeSelected, "Encode Selected", "Encode video file from images for selected job", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
