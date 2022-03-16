// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GamedevHelper.h"
#include "GamedevHelperTester.h"
#include "GamedevHelperAssetNamingManagerLibrary.h"
#include "GamedevHelperStringLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryNormalizeTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.Normalize",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryTokenizeTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.Tokenize",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FAssetNamingManagerLibraryNormalizeTest::RunTest(const FString& Parameters)
{
	// contracts
	// must start and end with alpha and digit characters
	// must not contain hyphens
	// must not contain trailing underscores

	const auto Scenario = []()
	{
		// input => expected
		TMap<FString, FString> Tests;
		Tests.Add(TEXT("_aaa_"), TEXT("aaa"));
		Tests.Add(TEXT("_aaa bc"), TEXT("aaa_bc"));
		Tests.Add(TEXT("__abc"), TEXT("abc"));
		Tests.Add(TEXT("abc__"), TEXT("abc"));
		Tests.Add(TEXT("_abc-def"), TEXT("abc_def"));
		Tests.Add(TEXT("_abc_def_"), TEXT("abc_def"));
		Tests.Add(TEXT("0123-45678-9088"), TEXT("0123_45678_9088"));
		Tests.Add(TEXT(""), TEXT(""));
		Tests.Add(TEXT("___"), TEXT(""));
		Tests.Add(TEXT("---"), TEXT(""));
		Tests.Add(TEXT("-_-"), TEXT(""));
		Tests.Add(TEXT("-default_Material-01"), TEXT("default_Material_01"));

		for (const auto& Test : Tests)
		{
			const FString Input = Test.Key;
			const FString Expected = Test.Value;
			const FString Actual = UGamedevHelperAssetNamingManagerLibrary::Normalize(Input);

			if (Actual != Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected normalized value for '%s', to be '%s', got '%s'"), *Input, *Expected, *Actual);
				return false;
			}
		}

		for (int32 i = 1; i < 100; ++i)
		{
			const FString RandomString = UGamedevHelperStringLibrary::GetRandomStringFromCharset(
				i,
				GamedevHelperConstants::AlphaMixed + GamedevHelperConstants::Digits + TEXT("`~!@#$%^&*()_-=+[]{};,.")
			);
			const FString Normalized = UGamedevHelperAssetNamingManagerLibrary::Normalize(RandomString);
			if (Normalized.StartsWith(TEXT("_")) || Normalized.EndsWith(TEXT("_")))
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Normalized '%s' must not start or end with '_' "), *Normalized);
				return false;
			}

			if (!UGamedevHelperStringLibrary::ContainsOnly(Normalized, GamedevHelperConstants::AlphaLower + GamedevHelperConstants::Digits + TEXT("_"), ESearchCase::IgnoreCase))
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Normalized '%s' must contain only letters, digits and underscore "), *Normalized);
				return false;
			}
		}
		return true;
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);

	return true;
}

bool FAssetNamingManagerLibraryTokenizeTest::RunTest(const FString& Parameters)
{
	// contracts

	const auto Scenario = []()
	{
		TMap<FString, FString> Tests;
		Tests.Add(TEXT("myMaterial"), TEXT("my_material"));
		Tests.Add(TEXT("abc_01"), TEXT("abc_01"));
		Tests.Add(TEXT("Abc02"), TEXT("abc_02"));
		Tests.Add(TEXT("01MyTexture02"), TEXT("01_my_texture_02"));
		Tests.Add(TEXT("-_weirdName-_023"), TEXT("weird_name_023"));
		Tests.Add(TEXT(""), TEXT(""));
		Tests.Add(TEXT("BFL-my-char"), TEXT("bfl_my_char"));
		Tests.Add(TEXT("a"), TEXT("a"));
		
		for (const auto& Test : Tests)
		{
			const FString Input = Test.Key;
			const FString Expected = Test.Value;
			const FString Actual = UGamedevHelperAssetNamingManagerLibrary::Tokenize(Input);

			if (Actual != Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected tokenized value for '%s', to be '%s', got '%s'"), *Input, *Expected, *Actual);
				return false;
			}
		}
		
		return true;	
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);
	return true;
}

