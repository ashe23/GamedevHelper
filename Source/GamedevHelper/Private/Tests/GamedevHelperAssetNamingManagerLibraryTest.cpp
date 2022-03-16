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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryConvertToPascalCaseTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.ConvertToPascalCase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryConvertToPascalSnakeCaseTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.ConvertToPascalSnakeCase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryConvertToSnakeCaseTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.ConvertToSnakeCase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryConvertToKebabCaseTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.ConvertToKebabCase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FAssetNamingManagerLibraryConvertToCamelCaseTest,
	"Plugins.GamedevHelper.Libraries.AssetNamingManager.ConvertToCamelCase",
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

bool FAssetNamingManagerLibraryConvertToPascalCaseTest::RunTest(const FString& Parameters)
{
	// input => expected
	TMap<FString, FString> Tests;
	Tests.Add(TEXT("my_material_01"), TEXT("MyMaterial01"));
	Tests.Add(TEXT("aaa"), TEXT("Aaa"));
	Tests.Add(TEXT("TEXTURE01"), TEXT("Texture01"));
	Tests.Add(TEXT("01My_Str"), TEXT("01MyStr"));
	Tests.Add(TEXT(""), TEXT(""));

	for (const auto& Test : Tests)
	{
		const FString Input = Test.Key;
		const FString Expected = Test.Value;
		const FString Actual = UGamedevHelperAssetNamingManagerLibrary::ConvertToPascalCase(Input);

		if (Expected != Actual)
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Expected PascalCase for '%s', to be '%s' got '%s' "), *Input, *Expected, *Actual);
			return false;
		}
	}
	return true;
}

bool FAssetNamingManagerLibraryConvertToPascalSnakeCaseTest::RunTest(const FString& Parameters)
{
	// input => expected
	TMap<FString, FString> Tests;
	Tests.Add(TEXT("my_material_01"), TEXT("My_Material_01"));
	Tests.Add(TEXT("aaa"), TEXT("Aaa"));
	Tests.Add(TEXT("TEXTURE01"), TEXT("Texture_01"));
	Tests.Add(TEXT("01My_Str"), TEXT("01_My_Str"));
	Tests.Add(TEXT(""), TEXT(""));

	for (const auto& Test : Tests)
	{
		const FString Input = Test.Key;
		const FString Expected = Test.Value;
		const FString Actual = UGamedevHelperAssetNamingManagerLibrary::ConvertToPascalSnakeCase(Input);

		if (Expected != Actual)
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Expected PascalSnakeCase for '%s', to be '%s' got '%s' "), *Input, *Expected, *Actual);
			return false;
		}
	}
	return true;
}

bool FAssetNamingManagerLibraryConvertToSnakeCaseTest::RunTest(const FString& Parameters)
{
	// input => expected
	TMap<FString, FString> Tests;
	Tests.Add(TEXT("my_material_01"), TEXT("my_material_01"));
	Tests.Add(TEXT("aaa"), TEXT("aaa"));
	Tests.Add(TEXT("TEXTURE01"), TEXT("texture_01"));
	Tests.Add(TEXT("01My_Str"), TEXT("01_my_str"));
	Tests.Add(TEXT(""), TEXT(""));

	for (const auto& Test : Tests)
	{
		const FString Input = Test.Key;
		const FString Expected = Test.Value;
		const FString Actual = UGamedevHelperAssetNamingManagerLibrary::ConvertToSnakeCase(Input);

		if (Expected != Actual)
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Expected snake_case for '%s', to be '%s' got '%s' "), *Input, *Expected, *Actual);
			return false;
		}
	}
	return true;
}

bool FAssetNamingManagerLibraryConvertToKebabCaseTest::RunTest(const FString& Parameters)
{
	// input => expected
	TMap<FString, FString> Tests;
	Tests.Add(TEXT("my_material_01"), TEXT("my-material-01"));
	Tests.Add(TEXT("aaa"), TEXT("aaa"));
	Tests.Add(TEXT("TEXTURE01"), TEXT("texture-01"));
	Tests.Add(TEXT("01My_Str"), TEXT("01-my-str"));
	Tests.Add(TEXT(""), TEXT(""));

	for (const auto& Test : Tests)
	{
		const FString Input = Test.Key;
		const FString Expected = Test.Value;
		const FString Actual = UGamedevHelperAssetNamingManagerLibrary::ConvertToKebabCase(Input);

		if (Expected != Actual)
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Expected kebab-case for '%s', to be '%s' got '%s' "), *Input, *Expected, *Actual);
			return false;
		}
	}
	return true;
}

bool FAssetNamingManagerLibraryConvertToCamelCaseTest::RunTest(const FString& Parameters)
{
	// input => expected
	TMap<FString, FString> Tests;
	Tests.Add(TEXT("my_material_01"), TEXT("myMaterial01"));
	Tests.Add(TEXT("aaa"), TEXT("aaa"));
	Tests.Add(TEXT("TEXTURE01"), TEXT("texture01"));
	Tests.Add(TEXT("01My_Str"), TEXT("01MyStr"));
	Tests.Add(TEXT(""), TEXT(""));

	for (const auto& Test : Tests)
	{
		const FString Input = Test.Key;
		const FString Expected = Test.Value;
		const FString Actual = UGamedevHelperAssetNamingManagerLibrary::ConvertToCamelCase(Input);

		if (Expected != Actual)
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Expected camelCase for '%s', to be '%s' got '%s' "), *Input, *Expected, *Actual);
			return false;
		}
	}
	return true;
}

