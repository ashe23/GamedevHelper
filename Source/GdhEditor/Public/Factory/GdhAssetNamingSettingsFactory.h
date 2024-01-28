// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhAssetNamingSettingsFactory.generated.h"

UCLASS()
class UGdhAssetNamingSettingsFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGdhAssetNamingSettingsFactory();
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
