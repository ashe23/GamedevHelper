// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperStringLibrary.h"
// Engine Headers
#include "Kismet/KismetStringLibrary.h"

FString UGamedevHelperStringLibrary::GetFirstLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, 0, 1);
}