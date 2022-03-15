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

	// main menu
	TSharedPtr<FUICommandInfo> Cmd_ProjectOrganizerWindow;
	
	// context menu
	// TSharedPtr<FUICommandInfo> CMD_VertexAnimToolStaticMesh;
	// TSharedPtr<FUICommandInfo> CMD_VertexAnimToolTextureUV;
	// TSharedPtr<FUICommandInfo> CMD_VertexAnimToolTextureNormal;
	// TSharedPtr<FUICommandInfo> CMD_NamingFix;
	// TSharedPtr<FUICommandInfo> CMD_NamingNumerize;
	// TSharedPtr<FUICommandInfo> CMD_UtilityDisableCollision;
	
	
};
