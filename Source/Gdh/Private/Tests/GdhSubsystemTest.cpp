#pragma once

#include "CoreMinimal.h"
#include "Gdh.h"
#include "Misc/AutomationTest.h"
#include "GdhSubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhSubsystemTest, "Gdh.Subsystem.Normalize", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace GdhSubsystemTests
{
	bool RunNormalizeTests()
	{
		// input => expected
		TMap<FString, FString> TestCases;
		TestCases.Add(TEXT(""), TEXT(""));
		TestCases.Add(TEXT("_"), TEXT(""));
		TestCases.Add(TEXT("-aa-"), TEXT("aa"));
		TestCases.Add(TEXT("PascalCase01"), TEXT("PascalCase01"));
		TestCases.Add(TEXT("01_my-string"), TEXT("01_my_string"));
		TestCases.Add(TEXT("TextureUV01"), TEXT("TextureUV01"));
		TestCases.Add(TEXT("aaa___bbb___ccc"), TEXT("aaa_bbb_ccc"));
		TestCases.Add(TEXT("-aa-Bb-cC--01-"), TEXT("aa_Bb_cC_01"));
		TestCases.Add(TEXT("`~!@#$%^&*()+={}[];.',*<>|"), TEXT(""));

		for (const auto& TestCase : TestCases)
		{
			const FString Input = TestCase.Key;
			const FString Expected = TestCase.Value;
			const FString Actual = UGdhSubsystem::Normalize(Input);

			if (!Actual.Equals(Expected, ESearchCase::CaseSensitive))
			{
				UE_LOG(LogGdh, Error, TEXT("[Normalize] Expected: %s, Got: %s for Input: %s"), *Expected, *Actual, *Input);
				return false;
			}
		}

		return true;
	}

	bool RunTokenizeTests()
	{
		// input => expected
		TMap<FString, FString> TestCases;
		TestCases.Add(TEXT(""), TEXT(""));
		TestCases.Add(TEXT("_"), TEXT(""));
		TestCases.Add(TEXT("-aa-"), TEXT("aa"));
		TestCases.Add(TEXT("-Aa-B-"), TEXT("aa_b"));
		TestCases.Add(TEXT("PascalCase01"), TEXT("pascal_case_01"));
		TestCases.Add(TEXT("01_my-string"), TEXT("01_my_string"));
		TestCases.Add(TEXT("TextureUV01"), TEXT("texture_uv_01"));
		TestCases.Add(TEXT("aaa___bbb___ccc"), TEXT("aaa_bbb_ccc"));
		TestCases.Add(TEXT("-aa-Bb-cC--01-"), TEXT("aa_bb_c_c_01"));
		TestCases.Add(TEXT("`~!@#$%^&*()+={}[];.',*<>|"), TEXT(""));

		for (const auto& TestCase : TestCases)
		{
			const FString Input = TestCase.Key;
			const FString Expected = TestCase.Value;
			const FString Actual = UGdhSubsystem::Tokenize(Input);

			if (!Actual.Equals(Expected, ESearchCase::CaseSensitive))
			{
				UE_LOG(LogGdh, Error, TEXT("[Tokenize] Expected: %s, Got: %s for Input: %s"), *Expected, *Actual, *Input);
				return false;
			}
		}

		return true;
	}

	bool RunPascalSnakeCaseTestCases()
	{
		// input => expected
		TMap<FString, FString> TestCases;
		// Additional Test Cases
		TestCases.Add(TEXT("myVariableName"), TEXT("My_Variable_Name")); // CamelCase
		TestCases.Add(TEXT("my_variable_name"), TEXT("My_Variable_Name")); // Snake_case
		TestCases.Add(TEXT("myTextureName"), TEXT("My_Texture_Name")); // CamelCase with mixed case
		TestCases.Add(TEXT("MyTextureName"), TEXT("My_Texture_Name")); // PascalCase
		TestCases.Add(TEXT("my_texture_Name"), TEXT("My_Texture_Name")); // Pascal Snake Case
		TestCases.Add(TEXT("my-texture-name"), TEXT("My_Texture_Name")); // Hyphens to Pascal Snake Case
		TestCases.Add(TEXT("my_texture_01_name"), TEXT("My_Texture_01_Name")); // Numbers included

		// Invalid String Test Cases
		TestCases.Add(TEXT("#Invalid!@String"), TEXT("Invalid_String")); // Special characters
		TestCases.Add(TEXT("12345"), TEXT("12345")); // Numeric input only
		
		for (const auto& TestCase : TestCases)
		{
			const FString Input = TestCase.Key;
			const FString Expected = TestCase.Value;
			const FString Actual = UGdhSubsystem::ConvertToPascalSnakeCase(Input);

			if (!Actual.Equals(Expected, ESearchCase::CaseSensitive))
			{
				UE_LOG(LogGdh, Error, TEXT("[PascalSnakeCase] Expected: %s, Got: %s for Input: %s"), *Expected, *Actual, *Input);
				return false;
			}
		}
		
		return true;
	}
}

bool FGdhSubsystemTest::RunTest(const FString& Parameters)
{
	return GdhSubsystemTests::RunNormalizeTests() && GdhSubsystemTests::RunTokenizeTests() && GdhSubsystemTests::RunPascalSnakeCaseTestCases();
}
