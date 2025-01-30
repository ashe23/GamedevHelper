// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhConstants.h"
#include "Framework/Commands/Commands.h"

class GDHEDITOR_API FGdhCmds final : public TCommands<FGdhCmds>
{
public:
	FGdhCmds() : TCommands(
		GdhConstants::ModuleFullName,
		FText::FromString("GamedevHelper Plugin"),
		NAME_None,
		GdhConstants::ModuleStylesName) {}

	virtual void RegisterCommands() override;

	// Main Menu
	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> OpenAssetNamingTool;
	TSharedPtr<FUICommandInfo> OpenActorNamingTool;
	TSharedPtr<FUICommandInfo> OpenVideoEncoderTool;

	// Asset Naming Tool
	TSharedPtr<FUICommandInfo> ScanAssets;
	TSharedPtr<FUICommandInfo> RenameAssets;
	TSharedPtr<FUICommandInfo> ClearSelection;
	TSharedPtr<FUICommandInfo> EditMode;
	TSharedPtr<FUICommandInfo> ApplyChanges;
	TSharedPtr<FUICommandInfo> UndoChanges;

	// Actor Naming Tool
	TSharedPtr<FUICommandInfo> ScanActors;
	TSharedPtr<FUICommandInfo> OrganizeActors;
	TSharedPtr<FUICommandInfo> RenameActors;
	TSharedPtr<FUICommandInfo> UndoAction;

	// Video Encoder Tool
	TSharedPtr<FUICommandInfo> RefreshPipelines;
	TSharedPtr<FUICommandInfo> Process;


};
