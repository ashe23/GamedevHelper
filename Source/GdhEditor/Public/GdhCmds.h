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
	TSharedPtr<FUICommandInfo> ScanAssets;
	TSharedPtr<FUICommandInfo> RenameAssets;
};
