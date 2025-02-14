// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhBetRenderListFactory.generated.h"

UCLASS()
class UGdhBetRenderListFactory : public UFactory
{
public:

	GENERATED_BODY()

	UGdhBetRenderListFactory();

	virtual UObject*
	FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
