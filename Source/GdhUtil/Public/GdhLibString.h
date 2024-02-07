// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibString.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName="StringLibrary"))
class GDHUTIL_API UGdhLibString : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns randomly generated string from given charset and seed
	 * @param Len int32
	 * @param Charset FString
	 * @param Seed int32
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetRandomStringFromCharset(const int32 Len, const FString& Charset, const int32 Seed = 0);
};
