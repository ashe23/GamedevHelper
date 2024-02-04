// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gdh.h"
#include "Framework/Commands/Commands.h"

class FGdhCommands final : public TCommands<FGdhCommands>
{
public:
	FGdhCommands() : TCommands(
		GdhConstants::ModuleFullName,
		FText::FromString("GamedevHelper Plugin"),
		NAME_None,
		GdhConstants::ModuleStylesName) {}

	virtual void RegisterCommands() override;

	// main menu bar
	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> OpenAssetNamingTool;
	TSharedPtr<FUICommandInfo> RenameAssets;
	TSharedPtr<FUICommandInfo> ScanAssets;
	TSharedPtr<FUICommandInfo> ClearSelection;

	// TSharedPtr<FUICommandInfo> Cmd_RestartEditor;
	// TSharedPtr<FUICommandInfo> Cmd_OpenAssetNamingManager;
	// TSharedPtr<FUICommandInfo> Cmd_OpenWorldOutlinearManager;
	// TSharedPtr<FUICommandInfo> Cmd_OpenMrqBatchRenderingManager;
	//
	// // asset naming manager
	// TSharedPtr<FUICommandInfo> Cmd_ScanAssets;
	// TSharedPtr<FUICommandInfo> Cmd_RenameAssets;
	// TSharedPtr<FUICommandInfo> Cmd_ClearSelection;
	//
	// // world outlinear manager
	// TSharedPtr<FUICommandInfo> Cmd_OrganizeActors;
	// TSharedPtr<FUICommandInfo> Cmd_Undo;
	// TSharedPtr<FUICommandInfo> Cmd_DeleteEmptyFolders;
};
