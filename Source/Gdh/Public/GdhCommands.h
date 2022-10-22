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
	TSharedPtr<FUICommandInfo> Cmd_OpenWindowRenderingManager;
	// TSharedPtr<FUICommandInfo> Cmd_OpenAssetNamingManagerWindow;
	// TSharedPtr<FUICommandInfo> Cmd_OpenWorldOutlinerManagerWindow;

	// context menu for rendering manager
	TSharedPtr<FUICommandInfo> Cmd_OpenImageDir;
	TSharedPtr<FUICommandInfo> Cmd_OpenVideoDir;
	TSharedPtr<FUICommandInfo> Cmd_PlayVideoFile;
	TSharedPtr<FUICommandInfo> Cmd_RemoveRenderedImages;
	TSharedPtr<FUICommandInfo> Cmd_RemoveEncodedVideo;
	TSharedPtr<FUICommandInfo> Cmd_RenderSelected;
	TSharedPtr<FUICommandInfo> Cmd_EncodeSelected;
	// TSharedPtr<FUICommandInfo> Cmd_AssetNamingManagerRenameSelected;
	// TSharedPtr<FUICommandInfo> Cmd_AssetNamingManagerOpenAsset;
};
