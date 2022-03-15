// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.generated.h"

UENUM(BlueprintType)
enum class EGamedevHelperNamingCase : uint8
{
	None,
	PascalCase UMETA(DisplayName = "PascalCase"),
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	KebabCase UMETA(DisplayName = "kebab-case"),
	CamelCase UMETA(DisplayName = "camelCase"),
};

UENUM(BlueprintType)
enum class EGamedevHelperLetterCase : uint8
{
	None,
	LowerCase UMETA(DiplayName = "lowercase"),
	UpperCase UMETA(DiplayName = "UPPERCASE"),
	MixedCase UMETA(DiplayName = "MiXeDCaSe"),
};

// UENUM(BlueprintType)
// enum class EGamedevHelperStringCase : uint8
// {
// 	None,
// 	Lower UMETA(DiplayName = "lowercase"),
// 	Upper UMETA(DiplayName = "UPPERCASE"),
// 	Mixed UMETA(DiplayName = "MiXeDCaSe"),
// 	StartUpper UMETA(DisplayName = "Startupper"),
// 	EndUpper UMETA(DisplayName = "enduppeR"),
// 	StartLower UMETA(DisplayName = "sTARTLOWER"),
// 	EndLower UMETA(DisplayName = "ENDLOWEr"),
// };

USTRUCT(BlueprintType)
struct FGamedevHelperAssetName
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetName")
	FString Prefix;
	
	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetName")
	FString Suffix;
};
