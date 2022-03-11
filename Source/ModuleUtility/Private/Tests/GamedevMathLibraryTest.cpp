// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GamedevMathLibrary.h"
#include "ModuleUtility.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGamedevHelperMathLibraryGetIntegerPartitionsTest,
	"Plugins.GamedevHelper.Utility.Math.GetIntegerPartitions",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FGamedevHelperMathLibraryGetIntegerPartitionsTest::RunTest(const FString& Parameters)
{
	TArray<int32> Parts;
	// original value is 0 or 1  or parts num is 0 or 1 => parts must be of length 1	
	UGamedevHelperMathLibrary::GetIntegerPartitions(0, 0, Parts);

	if (Parts.Num() != 1)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("[OriginalValue=0]Expected Parts num 1 got %d"), Parts.Num());
		return false;
	}

	Parts.Reset();
	UGamedevHelperMathLibrary::GetIntegerPartitions(1, 0, Parts);
	if (Parts.Num() != 1)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("[OriginalValue=1]Expected Parts num 1 got %d"), Parts.Num());
		return false;
	}

	Parts.Reset();
	UGamedevHelperMathLibrary::GetIntegerPartitions(10, 0, Parts);
	if (Parts.Num() != 1)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("[PartsNum=0]Expected Parts num 1 got %d"), Parts.Num());
		return false;
	}

	Parts.Reset();
	UGamedevHelperMathLibrary::GetIntegerPartitions(10, 1, Parts);
	if (Parts.Num() != 1)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("[PartsNum=1]Expected Parts num 1 got %d"), Parts.Num());
		return false;
	}

	// in all other cases parts num must be equal required parts num
	for (int32 i = 2; i < 50; ++i)
	{
		Parts.Reset();
		const int32 RequiredPartsNum = FMath::RandRange(1, 10);
		UGamedevHelperMathLibrary::GetIntegerPartitions(i, RequiredPartsNum, Parts);
		
		uint32 Sum = 0;
		for (const int32 Part : Parts)
		{
			Sum += Part;
		}

		if (Parts.Num() != RequiredPartsNum)
		{
			UE_LOG(LogModuleUtility, Error, TEXT("Input[%d] Expected parts %d got %d"), i, RequiredPartsNum, Parts.Num());
			return false;
		}

		if (Sum != i)
		{
			UE_LOG(LogModuleUtility, Error, TEXT("Input[%d] Expected parts sum to be %d got %d"), i, i, Sum);
			return false;
		}
	
	}

	return true;
}
