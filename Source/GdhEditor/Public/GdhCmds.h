// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhConstants.h"
#include "Framework/Commands/Commands.h"

class GDHEDITOR_API FGdhCmds final : public TCommands<FGdhCmds>
{
public:

	FGdhCmds() :
		TCommands(
			GdhConstants::ModuleFullName,
			FText::FromString("GamedevHelper Plugin"),
			NAME_None,
			GdhConstants::ModuleStylesName
		) {}

	virtual void RegisterCommands() override;

	// Main Menu
	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> OpenAssetNamingTool;
	TSharedPtr<FUICommandInfo> OpenActorNamingTool;
	TSharedPtr<FUICommandInfo> OpenBatchEncodeTool;

	// Asset Naming Tool
	// TSharedPtr<FUICommandInfo> ScanAssets;
	// TSharedPtr<FUICommandInfo> RenameAssets;
	// TSharedPtr<FUICommandInfo> ClearSelection;
	// TSharedPtr<FUICommandInfo> EditMode;
	// TSharedPtr<FUICommandInfo> ApplyChanges;
	// TSharedPtr<FUICommandInfo> UndoChanges;

	// Actor Naming Tool
	TSharedPtr<FUICommandInfo> AntScanActors;
	TSharedPtr<FUICommandInfo> AntOrganizeActors;
	TSharedPtr<FUICommandInfo> AntRenameActors;
	TSharedPtr<FUICommandInfo> AntUndoAction;

	// Batch Encoder Tool
	TSharedPtr<FUICommandInfo> BetRefresh;
	TSharedPtr<FUICommandInfo> BetProcess;
	TSharedPtr<FUICommandInfo> BetRenderListRefresh;
	TSharedPtr<FUICommandInfo> BetRenderListRemove;
	TSharedPtr<FUICommandInfo> BetEncodePresetRefresh;
	TSharedPtr<FUICommandInfo> BetEncodePresetCreate;
	TSharedPtr<FUICommandInfo> BetEncodePresetDuplicate;
	TSharedPtr<FUICommandInfo> BetEncodePresetRename;
	TSharedPtr<FUICommandInfo> BetEncodePresetRemove;
};
