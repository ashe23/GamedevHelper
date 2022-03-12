// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UtilityStringLibrary.h"

#include "Kismet/KismetStringLibrary.h"

FString UGamedevHelperStringLibrary::GetFirstLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, 0, 1);
}

FString UGamedevHelperStringLibrary::GetLastLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, OriginalString.Len() - 1, 1);
}
