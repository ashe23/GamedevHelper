// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhRenderListFactory.generated.h"

UCLASS()
class UGdhRenderListFactory : public UFactory
{
  public:

	GENERATED_BODY()

	UGdhRenderListFactory();

	virtual UObject* FactoryCreateNew(
		UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn
	) override;
};
