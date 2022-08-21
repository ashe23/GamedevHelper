// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGdhGenericStatus : uint8
{
	None UMETA(DisplayName = "None", Hidden),
	OK UMETA(DisplayName = "OK"),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error"),
};

UENUM(BlueprintType)
enum class EGdhModalStatus : uint8
{
	None UMETA(DisplayName = "None"),
	OK UMETA(DisplayName = "OK"),
	Pending UMETA(DisplayName = "Pending"),
	Error UMETA(DisplayName = "Error"),
};