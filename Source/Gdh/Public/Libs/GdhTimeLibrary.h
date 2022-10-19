// Copyright Ashot Barkhudaryan All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTimeLibrary.generated.h"

UCLASS()
class UGdhTimeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Time")
	static FString GetHumanReadableTime(const double Seconds);
};