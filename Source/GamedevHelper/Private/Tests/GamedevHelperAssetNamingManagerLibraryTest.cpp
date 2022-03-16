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
		
		return true;	
	};
	
	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);

	
	return true;
}
