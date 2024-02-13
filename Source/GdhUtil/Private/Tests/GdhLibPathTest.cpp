// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "GdhLibPath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibPath, "Gdh.Library.Path.Repeat", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FGdhLibPath::RunTest(const FString& Parameters)
{
	// todo:ashe23 normalize function must be more researched whats its actually doing
	TestEqual(TEXT("Empty String"), UGdhLibPath::Normalize(TEXT("")), TEXT(""));
	TestEqual(TEXT("Just slash #1"), UGdhLibPath::Normalize(TEXT("/")), TEXT(""));
	TestEqual(TEXT("Just slash #2"), UGdhLibPath::Normalize(TEXT("\\")), TEXT(""));
	TestEqual(TEXT("Just slash #3"), UGdhLibPath::Normalize(TEXT("C:")), TEXT(""));
	TestEqual(TEXT("Just slash #4"), UGdhLibPath::Normalize(TEXT("D:\\")), TEXT(""));
	TestEqual(TEXT("Just slash #5"), UGdhLibPath::Normalize(TEXT("D://")), TEXT(""));
	TestEqual(TEXT("No End slash"), UGdhLibPath::Normalize(TEXT("/Folder")), TEXT("/Folder"));
	TestEqual(TEXT("Duplicate Slashes"), UGdhLibPath::Normalize(TEXT("/Folder//AnotherFolder")), TEXT("/Folder/AnotherFolder"));
	TestEqual(TEXT("Ending With Slash"), UGdhLibPath::Normalize(TEXT("/Folder/")), TEXT("/Folder"));
	TestEqual(TEXT("No Whitespaces"), UGdhLibPath::Normalize(TEXT(" /Folder/ ")), TEXT(""));

	return true;
}


#endif
