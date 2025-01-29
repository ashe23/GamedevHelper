// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhSettingsEncoderAssetFactory.generated.h"

UCLASS()
class UGdhSettingsEncoderAssetFactory : public UFactory
{
	GENERATED_BODY()
  public:

	UGdhSettingsEncoderAssetFactory();
	virtual UObject* FactoryCreateNew(
		UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn
	) override;
};
