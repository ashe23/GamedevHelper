// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Fuzzer
{
	static const FString Alpha = TEXT("abcdefghijklmnopqrstuvwxyz");
	static const FString Digits = TEXT("0123456789");
	static const FString Specials = TEXT("_-");
	static const FString All = Alpha + Digits + Specials;

	enum class EStringFormat
	{
		None,
		// all lower case
		Lower,
		// all upper case
		Upper,
		// lower and upper case in random places
		Mixed,
		// only digits
		Digit,
		// only special chars
		Special,
		// First letter upper other lower
		StartUpper,
		// Last letter upper other lower
		EndUpper,
		// First letter lower other upper
		StartLower,
		// Last letter lower other upper
		EndLower,
		// PascalCase
		PascalCase,
		// Pascal_Snake_Case
		PascalSnakeCase,
		// kebab-case
		KebabCase,
		// snake_case
		SnakeCase,
		// camelCase
		CamelCase
	};
}

/**
 * @brief Responsible for generating random strings by given settings
 * @param Seed int32 Seed value for constant behavior
 */
struct FFuzzer
{
	FFuzzer(const int32 Seed = 0);

	/**
	 * @brief Returns randomly generated string
	 * @param Len int32 String length
	 * @param Dictionary FString Characters to chose from 
	 * @return FString
	 */
	FString GetRandomString(const int32 Len, const FString& Dictionary) const;
private:
	int32 Seed;
	FRandomStream RandomStream;
};
