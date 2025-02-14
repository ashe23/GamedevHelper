// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibEncoder.generated.h"

class UMoviePipelineMasterConfig;

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "GdhEncoderLibrary"))
class GDHEDITOR_API UGdhLibEncoder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static FString GetImageExtension(const UMoviePipelineMasterConfig* Config);
	static int32 GetZeroPadding(const UMoviePipelineMasterConfig* Config);
};
