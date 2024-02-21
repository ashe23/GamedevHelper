// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibString.generated.h"

enum class EGdhNamingCase : uint8;

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhStringLibrary"))
class GDHCORE_API UGdhLibString : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns lowercase alpha only characters
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetCharsetAlphaLower();

	/**
	 * @brief Returns uppercase alpha only characters
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetCharsetAlphaUpper();

	/**
	 * @brief Returns lowercase and uppercase alpha only characters
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetCharsetAlphaLowerAndUpper();

	/**
	 * @brief Returns digits only characters
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetCharsetDigits();

	/**
	 * @brief Returns alpha lower and upper case + digits
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetCharsetMixed();

	/**
	 * @brief Returns special characters
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString GetCharsetSpecial();

	// STRING CREATION

	/**
	 * @brief Creates a new string by repeating a string Num times
	 * @param Str FString
	 * @param Num int32
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String", meta=(DisplayName="Repeat String"))
	static FString Repeat(const FString& Str, const int32 Num);

	/**
	 * @brief Creates a new random string by given Len, Charset and Seed. By default Seed is zero, that will generate new seed every time internally.
	 * @param Len int32
	 * @param Charset FString
	 * @param Seed int32
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String", meta=(DisplayName="Random String"))
	static FString Random(const int32 Len, const FString& Charset, const int32 Seed = 0);

	// STRING CHECKS

	/**
	 * @brief Checks if given Str contains any characters in Charset
	 * @param Str FString
	 * @param Charset FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasAny(const FString& Str, const FString& Charset);

	/**
	 * @brief Checks if given Str does not contain any character in Charset
	 * @param Str FString
	 * @param Charset FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasNone(const FString& Str, const FString& Charset);

	/**
	 * @brief Checks if given Str contains only given Charset characters
	 * @param Str FString
	 * @param Charset FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasOnly(const FString& Str, const FString& Charset);

	/**
	 * @brief Checks if given Str contains any Ascii charset
	 * @param Str FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasAscii(const FString& Str);

	/**
	 * @brief Checks if given Str contains only Ascii charset
	 * @param Str FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasOnlyAscii(const FString& Str);

	/**
	 * @brief Checks if given Str contains any Unicode character
	 * @param Str FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasUnicode(const FString& Str);

	/**
	 * @brief Checks if given Str contains only Unicode characters
	 * @param Str FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static bool HasOnlyUnicode(const FString& Str);

	// STRING MANIPULATION

	/**
	 * @brief Returns intersection of character sets of given two strings
	 * - "", "" => ""
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
	 * - "", "" => ""
	 * - "", "abc" => "abc"
	 * - "abc", "def" => "abcdef"
	 * - "abc", "abcdef" => "abcdef"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Union(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns difference of character sets of given two string , set of all elements that are members of StringA, but not members of StringB
	 * - "", "" => ""
	 * - "abc", "abd" => "a"
	 * - "abc", "def" => "abc"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Difference(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns symmetric difference of character sets of given two string. Returns the elements that are unique to SetA or SetB, but not common to both.
	 * - "", "" => ""
	 * - "", "abc" => "abc"
	 * - "abc", "abc" => ""
	 * - "abc", "def" => "abcdef"
	 * - "abc", "abcdef" => "def"
	 * @param StringA FString
	 * @param StringB FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString SymmetricDifference(const FString& StringA, const FString& StringB);

	/**
	 * @brief Returns normalized string by removing all extra underscores and hyphens from string start and end, then replaces by underscore in the middle of string 
	 * - "_aa_" => "aa"
	 * - "aaa" => "aaa"
	 * - "aaa___bbb___ccc" => "aaa_bbb_ccc"
	 * - "-aa-bb-cc--01-" => "aa_bb_cc_01"
	 * 
	 * @param Str FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Normalize(const FString& Str);

	/**
	 * @brief Returns new string with contextual parts splitted by underscore. String can be in any naming case, even mixed.
	 * - "PascalCase01" => "pascal_case01"
	 * - "01_my-string" => "01_my_string"
	 * - "TextureUV01" => "texture_uv01"
	 * - "TextureUV_01" => "texture_uv_01"
	 * @param Str FString
	 * @return FString Result is always in snake_case
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString Tokenize(const FString& Str);

	/**
	 * @brief Converts string to given naming case
	 * @param Str FString
	 * @param NamingCase EGdhNamingCase
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertNamingCase(const FString& Str, const EGdhNamingCase NamingCase);

	/**
	 * @brief Converts string to PascalCase
	 * @param Str FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToPascalCase(const FString& Str);

	/**
	 * @brief Converts string to UPPERCASE, but this is tokenized version. For simple conversions use ToUpper
	 * @param Str FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToUpperCase(const FString& Str);

	/**
	 * @brief Converts string to lowercase, but this is tokenized version. For simple conversions use ToLower
	 * @param Str FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToLowerCase(const FString& Str);

	/**
	 * @brief Converts string to snake_case
	 * @param Str FString 
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToSnakeCase(const FString& Str);

	/**
	 * @brief Converts string to camelCase
	 * @param Str FString 
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToCamelCase(const FString& Str);

	/**
	 * @brief Converts string to kebab-case
	 * @param Str FString 
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToKebabCase(const FString& Str);

	/**
	 * @brief Converts string to Pascal_Snake_Case
	 * @param Str FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_String")
	static FString ConvertToPascalSnakeCase(const FString& Str);
};
