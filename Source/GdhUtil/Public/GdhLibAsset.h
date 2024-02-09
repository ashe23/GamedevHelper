// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibAsset.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhAssetLibrary"))
class GDHUTIL_API UGdhLibAsset : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
