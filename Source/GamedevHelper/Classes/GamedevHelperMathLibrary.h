// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevHelperMathLibrary.generated.h"

UCLASS()
class UGamedevHelperMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// /**
	//  * @brief Returns random partition for given integer
	//  * - @see https://en.wikipedia.org/wiki/Partition_(number_theory)
	//  * @param OriginalValue int32
	//  * @param PartsNum int32 Number of parts to generate
	//  * @param Parts Container for generated parts
	//  */
	// UFUNCTION(BlueprintCallable, Category="GamedevHelper|Math")
	// static void GetIntegerPartitions(const int32 OriginalValue, const int32 PartsNum, TArray<int32>& Parts);
};
