// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.generated.h"

UENUM(BlueprintType)
enum class EGdhNamingCase : uint8
{
	None,
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	PascalCase UMETA(DisplayName = "PascalCase"),
	UpperCase UMETA(DisplayName = "UPPERCASE"),
	LowerCase UMETA(DisplayName = "lowercase"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	CamelCase UMETA(DisplayName = "camelCase"),
	KebabCase UMETA(DisplayName = "kebab-case"),
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

UENUM(BlueprintType)
enum class EGdhGenericStatus : uint8
{
	None UMETA(DisplayName = "None", Hidden),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error"),
	OK UMETA(DisplayName = "OK")
};

UENUM(BlueprintType)
enum class EGdhImgFormat : uint8
{
	Png UMETA(DisplayName = "png"),
	Jpg UMETA(DisplayName = "jpg"),
	Bmp UMETA(DisplayName = "bmp"),
};

UENUM(BlueprintType)
enum class EGdhResolution : uint8
{
	Res360P UMETA(DisplayName = "360p (480x360)", ToolTip = "360p"),
	Res480P UMETA(DisplayName = "480p (640x480)", ToolTip = "480p"),
	Res720P UMETA(DisplayName = "720p (1280x720)", ToolTip = "720p (HD)"),
	Res1080P UMETA(DisplayName = "1080p (1920x1080)", ToolTip = "1080p (Full HD)"),
	Res1440P UMETA(DisplayName = "1440p (2560x1440)", ToolTip = "1440p (QHD)"),
	Res2160P UMETA(DisplayName = "2160p (3840x2160)", ToolTip = "2160p (4K)"),
	ResCustom UMETA(DisplayName = "Custom", ToolTip = "Custom resolution settings")
};
