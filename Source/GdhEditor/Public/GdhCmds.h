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

	// main menu bar
	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> OpenAssetNamingTool;

	// Asset Naming Tool
	TSharedPtr<FUICommandInfo> ScanAssets;
	TSharedPtr<FUICommandInfo> RenameAssets;
	TSharedPtr<FUICommandInfo> ClearSelection;
	TSharedPtr<FUICommandInfo> EditMode;
	TSharedPtr<FUICommandInfo> ApplyChanges;
	TSharedPtr<FUICommandInfo> UndoChanges;
	TSharedPtr<FUICommandInfo> NumerizeAssets;
	TSharedPtr<FUICommandInfo> SearchAndReplace;
};
