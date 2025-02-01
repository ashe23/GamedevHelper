// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhBetEncodePresetFactory.generated.h"

UCLASS()
class UGdhBetEncodePresetFactory : public UFactory
{
public:

	GENERATED_BODY()

	UGdhBetEncodePresetFactory();

	virtual UObject*
	FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
