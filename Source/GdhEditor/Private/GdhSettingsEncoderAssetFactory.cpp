// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSettingsEncoderAssetFactory.h"
#include "GdhSettingsEncoder.h"

UGdhSettingsEncoderAssetFactory::UGdhSettingsEncoderAssetFactory() {
	SupportedClass = UGdhSettingsEncoder::StaticClass();
	bCreateNew = true;
}

UObject* UGdhSettingsEncoderAssetFactory::FactoryCreateNew(
	UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn
) {
	return NewObject<UGdhSettingsEncoder>(InParent, InClass, InName, Flags, Context);
}
