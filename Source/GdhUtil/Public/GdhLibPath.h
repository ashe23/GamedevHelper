// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibPath.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhPathLibrary"))
class GDHUTIL_API UGdhLibPath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Normalize given path
	 * @param InPath FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Path")
	static FString Normalize(const FString& InPath);
};
