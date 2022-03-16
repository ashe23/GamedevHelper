// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperAssetNamingManagerLibrary.h"
#include "GamedevHelperStringLibrary.h"
// Engine Headers
#include "Kismet/KismetStringLibrary.h"

void UGamedevHelperAssetNamingManagerLibrary::RenameAsset(const FAssetData& Asset, const UGamedevHelperAssetNamingManagerSettings* Settings)
{
	// todo:ashe23
}

void UGamedevHelperAssetNamingManagerLibrary::RenameAssets(const TArray<FAssetData>& Assets, const UGamedevHelperAssetNamingManagerSettings* Settings)
{
	// todo:ashe23
}

FString UGamedevHelperAssetNamingManagerLibrary::Normalize(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	// replace all non alphabet and digit characters with underscore
	FString CleanedString = UGamedevHelperStringLibrary::RegexReplace(OriginalString, TEXT("[^0-9a-zA-Z]+"), TEXT("_"));
	CleanedString.RemoveFromStart(TEXT("_"));
	CleanedString.RemoveFromEnd(TEXT("_"));
	return CleanedString;
}

FString UGamedevHelperAssetNamingManagerLibrary::Tokenize(const FString& OriginalString)
{
	// todo:ashe23
	return FString{};
}

FString UGamedevHelperAssetNamingManagerLibrary::ConvertNamingCase(const FString& OriginalString, const EGamedevHelperNamingCase NamingCase)
{
	// todo:ashe23
	return FString{};
}
