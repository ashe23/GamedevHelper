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
	 * @brief Returns intersection of character sets of given two strings
	 * - "abc", "abd" => "ab"
	 * - "abcdef" , "f" => "f"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString Intersection(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns union of character sets of given two strings
	 * - "abc", "def" => "abcdef"
	 * - "abc", "abcdef" => "abcdef"
	 * - "", "abc" => "abc"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString Union(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns symmetric difference of character sets of given two string
	 * - "abc", "def" => "abcdef"
	 * - "abc", "abcdef" => "def"
	 * - "", "abc" => "abc"
	 * - "", "" => ""
	 * @param StringA 
	 * @param StringB 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString SymmetricDifference(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns difference of character sets of given two string
	 * - "abc", "abd" => "c"
	 * - "abc", "def" => ""
	 * @param StringA 
	 * @param StringB 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static FString Difference(const FString& StringA, const FString& StringB);

	/**
	 * @brief Checks if stringA character set is subset of stringB character set
	 * @param StringA FString
	 * @param StringB FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool IsSubSet(const FString& StringA, const FString& StringB);

	/**
	 * @brief Checks if string contains any of characters in given dictionary
	 * @param OriginalString FString
	 * @param Dictionary FString
	 * @param SearchCase ESearchCase
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool Contains(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase);

	/**
	 * @brief Checks if string contains ONLY characters from given dictionary
	 * @param OriginalString FString
	 * @param Dictionary FString
	 * @param SearchCase ESearchCase
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool ContainsOnly(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase);

	/**
	 * @brief Checks if string contains letters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool ContainsLetters(const FString& OriginalString);

	/**
	 * @brief Checks if string contains digits
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool ContainsDigits(const FString& OriginalString);

	/**
	 * @brief Checks if string contains only letters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool ContainsOnlyLetters(const FString& OriginalString);

	/**
	 * @brief Checks if string contains only digits
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|String")
	static bool ContainsOnlyDigits(const FString& OriginalString);
};
