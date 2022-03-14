// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return static_cast<EnumType>(Enum->GetIndexByName(FName(*String)));
}

UENUM(BlueprintType)
enum class EGDHBlueprintType : uint8
{
	None,
	Normal,
	Interface,
	MacroLibrary,
	FunctionLibrary
};

UENUM(BlueprintType)
enum class EGDHVertexAnimToolTextureType : uint8
{
	UV,
	Normal
};

UENUM(BlueprintType)
enum class EGDHNamingCase : uint8
{
	PascalCase UMETA(DisplayName = "PascalCase"),
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	KebabCase UMETA(DisplayName = "kebab-case"),
	CamelCase UMETA(DisplayName = "camelCase"),
};

UENUM(BlueprintType)
enum class EGDHFuzzerStringFormat : uint8
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
