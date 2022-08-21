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