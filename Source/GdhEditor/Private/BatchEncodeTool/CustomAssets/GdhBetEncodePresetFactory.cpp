// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/CustomAssets/GdhBetEncodePresetFactory.h"
#include "BatchEncodeTool/CustomAssets/GdhBetEncodePreset.h"

UGdhBetEncodePresetFactory::UGdhBetEncodePresetFactory() {
	SupportedClass = UGdhBetEncodePreset::StaticClass();
	bCreateNew = true;
}

UObject* UGdhBetEncodePresetFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
) {
	return NewObject<UGdhBetEncodePreset>(InParent, InClass, InName, Flags, Context);
}
