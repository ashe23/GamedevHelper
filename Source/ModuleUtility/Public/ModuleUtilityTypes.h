// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleUtilityTypes.generated.h"

UENUM(BlueprintType)
enum class EGDHBlueprintType : uint8
{
	None,
	Normal,
	Const,
	Interface,
	MacroLibrary,
	FunctionLibrary
};
