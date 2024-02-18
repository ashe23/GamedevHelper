// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "GdhLibPath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibPath, "Gdh.Library.Path.Repeat", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FGdhLibPath::RunTest(const FString& Parameters)
{
	// Test case 1: Empty path
	TestEqual(TEXT("Normalize empty path"), UGdhLibPath::Normalize(TEXT("")), TEXT(""));

	// Test case 2: Path starts with a slash
	TestEqual(TEXT("Normalize path starting with slash"), UGdhLibPath::Normalize(TEXT("/User/Documents")), TEXT("C:/User/Documents")); // Adjust expected output based on your logic

	// Test case 3: Path starts with backslash
	TestEqual(TEXT("Normalize path starting with backslash"), UGdhLibPath::Normalize(TEXT("\\User\\Documents")), TEXT("C:/User/Documents")); // Adjust expected output based on your logic

	// Test case 4: Relative path
	TestEqual(TEXT("Normalize relative path"), UGdhLibPath::Normalize(TEXT("../Documents")), TEXT("C:/User/Documents")); // Adjust based on your logic
	// todo:ashe23 normalize function must be more researched whats its actually doing
	// TestEqual(TEXT("Empty String"), UGdhLibPath::Normalize(TEXT("")), TEXT(""));
	// TestEqual(TEXT("Just slash #1"), UGdhLibPath::Normalize(TEXT("/")), TEXT(""));
	// TestEqual(TEXT("Just slash #2"), UGdhLibPath::Normalize(TEXT("\\")), TEXT(""));
	// TestEqual(TEXT("Just slash #3"), UGdhLibPath::Normalize(TEXT("C:")), TEXT(""));
	// TestEqual(TEXT("Just slash #4"), UGdhLibPath::Normalize(TEXT("D:\\")), TEXT(""));
	// TestEqual(TEXT("Just slash #5"), UGdhLibPath::Normalize(TEXT("D://")), TEXT(""));
	// TestEqual(TEXT("No End slash"), UGdhLibPath::Normalize(TEXT("/Folder")), TEXT("/Folder"));
	// TestEqual(TEXT("Duplicate Slashes"), UGdhLibPath::Normalize(TEXT("/Folder//AnotherFolder")), TEXT("/Folder/AnotherFolder"));
	// TestEqual(TEXT("Ending With Slash"), UGdhLibPath::Normalize(TEXT("/Folder/")), TEXT("/Folder"));
	// TestEqual(TEXT("No Whitespaces"), UGdhLibPath::Normalize(TEXT(" /Folder/ ")), TEXT(""));

	return true;
}


#endif
