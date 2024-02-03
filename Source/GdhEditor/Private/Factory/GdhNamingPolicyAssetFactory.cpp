// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Factory/GdhNamingPolicyAssetFactory.h"
#include "Asset/GdhNamingPolicyAsset.h"

UGdhNamingPolicyAssetFactory::UGdhNamingPolicyAssetFactory()
{
	SupportedClass = UGdhNamingPolicyAsset::StaticClass();
	bCreateNew = true;
}

UObject* UGdhNamingPolicyAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UGdhNamingPolicyAsset>(InParent, Class, Name, Flags, Context);
}
