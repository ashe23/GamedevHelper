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
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString GetLastLetter(const FString& OriginalString);


	/**
	 * @brief Removes first letter of given string
	 * - "abc" => "bc"
	 * - "a" = > ""
	 * - "" => ""
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString RemoveFirstLetter(const FString& OriginalString);

	/**
	 * @brief Removes last letter of given string
	 * - "abc" => "ab"
	 * - "a" => ""
	 * - "" => ""
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString RemoveLastLetter(const FString& OriginalString);

	/**
	 * @brief Returns intersection of characters of given two strings
	 * - "abc", "abd" => "ab"
	 * - "abcdef" , "f" => "f"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString Intersection(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns union of characters of given two strings
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString Union(const FString& StringA, const FString& StringB);
};
