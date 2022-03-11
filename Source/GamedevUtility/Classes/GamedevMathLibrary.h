// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevMathLibrary.generated.h"

UCLASS()
class UGamedevHelperMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Math")
	static void GetIntegerPartitions(const int32 OriginalValue, const int32 PartsNum, TArray<int32>& Parts);
};
