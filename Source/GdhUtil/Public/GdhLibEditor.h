// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibEditor.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhEditorLibrary"))
class GDHUTIL_API UGdhLibEditor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
