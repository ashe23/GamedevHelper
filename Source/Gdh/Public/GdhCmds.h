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
		GdhConstants::ModuleStylesName) { }

	virtual void RegisterCommands() override;

	// main menu bar
	TSharedPtr<FUICommandInfo> Cmd_RestartEditor;
};
