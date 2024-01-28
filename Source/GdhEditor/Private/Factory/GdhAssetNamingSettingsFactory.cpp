// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Factory/GdhAssetNamingSettingsFactory.h"
#include "Asset/GdhAssetNamingSettings.h"

UGdhAssetNamingSettingsFactory::UGdhAssetNamingSettingsFactory()
{
	SupportedClass = UGdhAssetNamingSettings::StaticClass();
	bCreateNew = true;
}

UObject* UGdhAssetNamingSettingsFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UGdhAssetNamingSettings>(InParent, Class, Name, Flags, Context);
}
