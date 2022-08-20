// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GdhCoreDeveloperSettings.generated.h"

UCLASS()
class GDHCORE_API UGdhDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UGdhDeveloperSettings() : bIsValid(false), ErrorMsg(TEXT(""))
	{
	}

	virtual void Validate()
	{
	};

	bool IsValid() const
	{
		return bIsValid;
	};

	FString GetErrorMsg() const
	{
		return ErrorMsg;
	}

	void SetErrorMsg(const FString& Msg)
	{
		if (Msg.IsEmpty()) return;

		ErrorMsg = Msg;
	}

private:
	bool bIsValid;
	FString ErrorMsg;
};
