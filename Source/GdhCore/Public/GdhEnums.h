// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.generated.h"

UENUM(BlueprintType)
enum class EGdhNamingCase : uint8
{
	None,
	PascalSnakeCase UMETA(DisplayName="Pascal_Snake_Case"),
	PascalCase UMETA(DisplayName="PascalCase"),
	UpperCase UMETA(DisplayName="UPPERCASE"),
	LowerCase UMETA(DisplayName="lowercase"),
	SnakeCase UMETA(DisplayName="snake_case"),
	CamelCase UMETA(DisplayName="camelCase"),
	KebabCase UMETA(DisplayName="kebab-case"),
};

UENUM(BlueprintType)
enum class EGdhAssetNameDelimiter : uint8
{
	Underscore UMETA(DisplayName="Underscore ( _ )"),
	Hyphen UMETA(DisplayName="Hyphen ( - )"),
};

UENUM(BlueprintType)
enum class EGdhBlueprintType : uint8
{
	None,
	Normal,
	Interface,
	FunctionLibrary,
	MacroLibrary
};
