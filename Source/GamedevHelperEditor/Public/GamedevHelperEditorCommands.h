// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FGamedevHelperEditorCommands : public TCommands<FGamedevHelperEditorCommands>
{
public:
	FGamedevHelperEditorCommands() : TCommands<FGamedevHelperEditorCommands>(
		TEXT("GamedevHelper"),
		NSLOCTEXT("Contexts", "GamedevHelper", "GamedevHelper Plugin"),
		NAME_None,
		FName{"GamedevHelperStyle"})
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> CMD_ProjectOrganizerWindow;
};
