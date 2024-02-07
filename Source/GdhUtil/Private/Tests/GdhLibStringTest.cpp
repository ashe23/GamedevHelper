#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GdhLibString.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibString, "Gdh.Library.String", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace GdhLibStringTests
{
	bool RunGetRandomStringFromCharsetTests()
	{
		// GetRandomStringFromCharset Tests
		// 1. if len = 0 return empty string
		// 2. if len > 0 and charset empty return empty string
		// 3. if len > 0 and charset not empty must return string with given len and contain only charset digits
		// 4. if same parameters given with seed, should return same string

		FString TestValue = UGdhLibString::GetRandomStringFromCharset(0, TEXT(""), 0);
		if (!TestValue.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("[GetRandomStringFromCharset] Expected to get empty string, got %s"), *TestValue);
			return false;
		}

		TestValue = UGdhLibString::GetRandomStringFromCharset(1, TEXT(""), 0);
		if (!TestValue.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("[GetRandomStringFromCharset] Expected to get empty string, got %s"), *TestValue);
			return false;
		}

		TestValue = UGdhLibString::GetRandomStringFromCharset(1, TEXT("abc"), 0);

		if (TestValue.Len() != 1)
		{
			UE_LOG(LogTemp, Error, TEXT("[GetRandomStringFromCharset] Expected to get string with len 1, got %d"), TestValue.Len());
			return false;
		}

		for (int32 i = 1; i < 100; ++i)
		{
			TestValue.Reset();
			TestValue = UGdhLibString::GetRandomStringFromCharset(i, TEXT("abcABC"), 0);

			if (TestValue.Len() != i)
			{
				UE_LOG(LogTemp, Error, TEXT("[GetRandomStringFromCharset] Expected to get string with len %d, got %d -> %s"), i, TestValue.Len(), *TestValue);
				return false;
			}
		}

		const FString Charset = TEXT("abcdef");
		constexpr int32 Len = 3;
		constexpr int32 Seed = 23;

		const FString Res1 = UGdhLibString::GetRandomStringFromCharset(Len, Charset, Seed);
		const FString Res2 = UGdhLibString::GetRandomStringFromCharset(Len, Charset, Seed);

		if (!Res1.Equals(Res2, ESearchCase::CaseSensitive))
		{
			UE_LOG(LogTemp, Error, TEXT("[GetRandomStringFromCharset] Expected string with given seed and params have same value, got different %s -> %s"), *Res1, *Res2);
			return false;
		}

		return true;
	}
}

bool FGdhLibString::RunTest(const FString& Parameters)
{
	return GdhLibStringTests::RunGetRandomStringFromCharsetTests();
}
