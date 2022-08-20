// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FGdhEditorCommands : public TCommands<FGdhEditorCommands>
{
public:
	FGdhEditorCommands() : TCommands<FGdhEditorCommands>(
		TEXT("GamedevHelper"),
		NSLOCTEXT("Contexts", "GamedevHelper", "GamedevHelper Plugin"),
		NAME_None,
		FName{"GamedevHelperStyle"})
	{
	}

	virtual void RegisterCommands() override;

	// main menu
	TSharedPtr<FUICommandInfo> Cmd_RestartEditor;
	// TSharedPtr<FUICommandInfo> Cmd_OpenAssetNamingManagerWindow;
	// TSharedPtr<FUICommandInfo> Cmd_OpenWorldOutlinerManagerWindow;
	TSharedPtr<FUICommandInfo> Cmd_OpenRenderingManagerWindow;

	// context menu
	// TSharedPtr<FUICommandInfo> Cmd_AssetNamingManagerRenameSelected;
	// TSharedPtr<FUICommandInfo> Cmd_AssetNamingManagerOpenAsset;
	// TSharedPtr<FUICommandInfo> Cmd_RenderingManagerRemoveRenderedImages;
	// TSharedPtr<FUICommandInfo> Cmd_RenderingManagerRemoveRenderedVideo;
};
