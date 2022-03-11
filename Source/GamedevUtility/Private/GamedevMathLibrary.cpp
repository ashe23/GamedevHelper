// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevUtility/Classes/GamedevMathLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UGamedevHelperMathLibrary::GetIntegerPartitions(const int32 OriginalValue, const int32 PartsNum, TArray<int32>& Parts)
{
	// https://en.wikipedia.org/wiki/Partition_(number_theory)
	
	if (PartsNum <= 1 || OriginalValue <= 1)
	{
		Parts.Add(OriginalValue);
		return;
	}
	
	
	Parts.Reserve(PartsNum);

	int32 Sum = OriginalValue;
	for (int32 i = 0; i < PartsNum; ++i)
	{
		constexpr uint32 Min = 1;
		constexpr uint32 Max = 9;
		const uint32 SMin = (PartsNum - i - 1) * Min;
		const uint32 SMax = (PartsNum - i - 1) * Max;
		const uint32 Offset = FMath::Max(Sum - SMax, Min);
		const uint32 RandomValue = 1 + FMath::Min3(Sum - Offset, Max - Offset, Sum - SMin - Min);
		const uint32 Value = FMath::Floor(UKismetMathLibrary::RandomFloat() * RandomValue + Offset);
		Sum -= Value;
		Parts.Add(Value);
	}
}
