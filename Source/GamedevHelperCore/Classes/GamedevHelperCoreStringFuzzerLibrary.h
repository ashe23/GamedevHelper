// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevHelperCoreTypes.h"
#include "GamedevHelperCoreStringFuzzerLibrary.generated.h"

/**
 * @brief Responsible for generating random strings by given settings
 */
UCLASS()
class UGamedevHelperStringFuzzerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Returns randomly generated string
	 * @param Len int32 String length
	 * @param Dictionary FString Characters to chose from
	 * @param Seed int32
	 * @return FString
	 */
	static FString GetRandomString(const int32 Len, const FString& Dictionary, const int32 Seed);

	/**
	 * @brief Returns randomly generated string with given string and string format 
	 * @param Len int32 
	 * @param StringFormat EFuzzerStringFormat
	 * @param Seed int32
	 * @return FString
	 */
	static FString GetRandomString(const int32 Len, const EGDHFuzzerStringFormat StringFormat, const int32 Seed);
};