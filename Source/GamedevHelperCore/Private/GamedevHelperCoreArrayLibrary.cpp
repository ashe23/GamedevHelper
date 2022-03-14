// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperCoreArrayLibrary.h"

void UGamedevHelperCoreArrayLibrary::Array_ShuffleWithSeed(const TArray<int32>& TargetArray, const int32 Seed)
{
	check(0);
}

void UGamedevHelperCoreArrayLibrary::GenericArray_ShuffleWithSeed(void* TargetArray, const FArrayProperty* ArrayProp, const int32 Seed)
{
	if (!TargetArray)
	{
		return;
	}

	const FRandomStream Stream{ Seed };

	FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
	const int32 LastIndex = ArrayHelper.Num() - 1;

	for (int32 i = 0; i <= LastIndex; ++i)
	{
		const int32 Index = Stream.RandRange(i, LastIndex);
		if (i != Index)
		{
			ArrayHelper.SwapValues(i, Index);
		}
	}
}