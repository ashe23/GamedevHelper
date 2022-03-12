// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityStringLibrary.generated.h"

UCLASS()
class UGamedevHelperStringLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns first letter of given string
	 * - "abc" => "a"
	 * - "" => ""
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString GetFirstLetter(const FString& OriginalString);

	/**
	 * @brief Returns last letter of given string
	 * - "abc" => "c"
	 * - "" => ""
	 * @param OriginalString FString
	 * @return FString
	 */
	static FString GetLastLetter(const FString& OriginalString);
};
