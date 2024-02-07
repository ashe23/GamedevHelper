// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibString.generated.h"

enum class EGdhNamingCase : uint8;

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhStringLibrary"))
class GDHUTIL_API UGdhLibString : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns intersection of character sets of given two strings
	 * - "abc", "abd" => "ab"
	 * - "abcdef" , "f" => "f"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Union(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns symmetric difference of character sets of given two string
	 * - "abc", "def" => "abcdef"
	 * - "abc", "abcdef" => "def"
	 * - "", "abc" => "abc"
	 * - "", "" => ""
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString SymmetricDifference(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns difference of character sets of given two string , set of all elements that are members of StringA, but not members of StringB
	 * - "abc", "abd" => "a"
	 * - "abc", "def" => "abc"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Difference(const FString& StringA, const FString& StringB);

	/**
	 * @brief Checks if stringA character set is subset of stringB character set
	 * @param StringA FString
	 * @param StringB FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool IsSubSet(const FString& StringA, const FString& StringB);

	/**
	 * @brief Checks if string contains any of characters in given dictionary
	 * @param OriginalString FString
	 * @param Dictionary FString
	 * @param SearchCase ESearchCase
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool Contains(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase);

	/**
	 * @brief Similar to Contains, but checks if string contains ONLY characters from given dictionary
	 * @param OriginalString FString
	 * @param Dictionary FString
	 * @param SearchCase ESearchCase
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsOnly(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase);

	/**
	 * @brief Checks if string contains letters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsLetters(const FString& OriginalString);

	/**
	 * @brief Checks if string contains digits
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsDigits(const FString& OriginalString);

	/**
	 * @brief Checks if string contains only letters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsOnlyLetters(const FString& OriginalString);

	/**
	 * @brief Checks if string contains only digits
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsOnlyDigits(const FString& OriginalString);

	/**
	 * @brief Checks if string contains ascii characters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsAscii(const FString& OriginalString);

	/**
	 * @brief Checks if string contains unicode characters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsUnicode(const FString& OriginalString);

	/**
	 * @brief Checks if string contains ONLY ascii characters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsOnlyAscii(const FString& OriginalString);

	/**
	 * @brief Checks if string contains ONLY ascii characters
	 * @param OriginalString FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool ContainsOnlyUnicode(const FString& OriginalString);

	/**
	 * @brief Returns randomly generated string from given charset and seed
	 * @param Len int32
	 * @param Charset FString
	 * @param Seed int32
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetRandomStringFromCharset(const int32 Len, const FString& Charset, const int32 Seed = 0);

	/**
	 * @brief Returns normalized string by removing all extra underscores and hyphens from string start and end, then replaces by underscore in the middle of string 
	 * - _aa_ => aa
	 * - aaa => aaa
	 * - aaa___bbb___ccc => aaa_bbb_ccc
	 * - -aa-bb-cc--01- => aa_bb_cc_01
	 * 
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Normalize(const FString& OriginalString);

	/**
	 * @brief Returns new string with contextual parts splitted by underscore. String can be in any naming case, even mixed.
	 * - PascalCase01 => pascal_case_01
	 * - 01_my-string => 01_my_string
	 * - TextureUV01 => texture_uv_01
	 * @param OriginalString FString
	 * @return FString Result is always in snake_case
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Tokenize(const FString& OriginalString);

	/**
	 * @brief Converts string to given naming case
	 * @param OriginalString FString
	 * @param NamingCase EGdhNamingCase
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertNamingCase(const FString& OriginalString, const EGdhNamingCase NamingCase);

	/**
	 * @brief Converts string to PascalCase
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToPascalCase(const FString& OriginalString);

	/**
	 * @brief Converts string to UPPERCASE
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToUpperCase(const FString& OriginalString);

	/**
	 * @brief Converts string to lowercase
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToLowerCase(const FString& OriginalString);

	/**
	 * @brief Converts string to snake_case
	 * @param OriginalString FString 
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToSnakeCase(const FString& OriginalString);

	/**
	 * @brief Converts string to camelCase
	 * @param OriginalString FString 
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToCamelCase(const FString& OriginalString);

	/**
	 * @brief Converts string to kebab-case
	 * @param OriginalString FString 
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToKebabCase(const FString& OriginalString);

	/**
	 * @brief Converts string to Pascal_Snake_Case
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToPascalSnakeCase(const FString& OriginalString);
};
