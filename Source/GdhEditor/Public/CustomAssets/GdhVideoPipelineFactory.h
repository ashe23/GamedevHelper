// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhVideoPipelineFactory.generated.h"

UCLASS()
class UGdhVideoPipelineFactory : public UFactory
{
  public:

	GENERATED_BODY()

	UGdhVideoPipelineFactory();

	virtual UObject* FactoryCreateNew(
		UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn
	) override;
};
