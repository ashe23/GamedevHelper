// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevMathLibrary.generated.h"

UCLASS()
class MODULEUTILITY_API UGamedevHelperMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Returns random partitions for given integer
	 * - Input 4 [2,2] or [1,2,1] etc
	 * - @see https://en.wikipedia.org/wiki/Partition_(number_theory)
	 * @param OriginalValue int32
	 * @param PartsNum int32 Number of parts to generate
	 * @param Parts Container for generated parts
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Math")
	static void GetIntegerPartitions(const int32 OriginalValue, const int32 PartsNum, TArray<int32>& Parts);
};
