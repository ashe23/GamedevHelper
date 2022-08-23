// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gdh.h"
#include "Framework/Commands/Commands.h"

class FGdhCommands : public TCommands<FGdhCommands>
{
public:
	FGdhCommands() : TCommands<FGdhCommands>(
		GdhConstants::ModuleFullName,
		FText::FromString("GamedevHelper Plugin"),
		NAME_None,
		GdhConstants::ModuleStylesName)
	{
	}

	virtual void RegisterCommands() override;

	// main menu
	TSharedPtr<FUICommandInfo> Cmd_RestartEditor;
	// TSharedPtr<FUICommandInfo> Cmd_OpenAssetNamingManagerWindow;
	// TSharedPtr<FUICommandInfo> Cmd_OpenWorldOutlinerManagerWindow;
	TSharedPtr<FUICommandInfo> Cmd_OpenWindowRenderingManager;

	// context menu
	// TSharedPtr<FUICommandInfo> Cmd_AssetNamingManagerRenameSelected;
	// TSharedPtr<FUICommandInfo> Cmd_AssetNamingManagerOpenAsset;
	// TSharedPtr<FUICommandInfo> Cmd_RenderingManagerRemoveRenderedImages;
	// TSharedPtr<FUICommandInfo> Cmd_RenderingManagerRemoveRenderedVideo;
};
