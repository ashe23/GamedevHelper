// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GdhNamingPolicyAssetFactory.generated.h"

UCLASS()
class UGdhNamingPolicyAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGdhNamingPolicyAssetFactory();
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
